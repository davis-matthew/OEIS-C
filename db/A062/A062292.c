/*
 * A062292  A B_2 sequence: a(n) is the smallest cube such that the pairwise
 *          sums of {a(1)...a(n)} are all distinct.
 * https://oeis.org/A062292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 34, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   hash_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the smallest cube such that the set of pairwise sums of
 * the selected cubes remains pairwise distinct, found by a greedy construction.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/hash_func.h"

/* ------- Hash table for sums (open addressing, linear probing) ------- */
static seq_t *hash_table = NULL;  /* array of keys, 0 denotes empty slot   */
static size_t hash_cap   = 0;      /* table size – power of two            */
static size_t hash_cnt   = 0;      /* number of stored keys                */

/* initialise the hash table with a small capacity */
static void init_hash(void)
{
    hash_cap   = 64; /* initial size, power of two */
    hash_table = (seq_t *)calloc(hash_cap, sizeof(seq_t));
}

/* rehash into a larger table (double the capacity) */
static void resize_hash(void)
{
    size_t new_cap = hash_cap << 1;
    seq_t *new_tbl = (seq_t *)calloc(new_cap, sizeof(seq_t));

    for (size_t i = 0; i < hash_cap; ++i) {
        seq_t k = hash_table[i];
        if (k != (seq_t)0) {
            size_t h = hash_func(k, new_cap);
            while (new_tbl[h] != (seq_t)0)
                h = (h + 1) & (new_cap - 1);
            new_tbl[h] = k;
        }
    }

    free(hash_table);
    hash_table = new_tbl;
    hash_cap   = new_cap;
}

/* test whether a key is already stored */
static bool hash_contains(seq_t key)
{
    if (hash_cnt == 0)
        return false;
    size_t h = hash_func(key, hash_cap);
    while (hash_table[h] != (seq_t)0) {
        if (hash_table[h] == key)
            return true;
        h = (h + 1) & (hash_cap - 1);
    }
    return false;
}

/* insert a new key; grow the table if load factor > 0.5 */
static void hash_insert(seq_t key)
{
    if ((hash_cnt << 1) >= hash_cap)   /* too many items – resize   */
        resize_hash();

    size_t h = hash_func(key, hash_cap);
    while (hash_table[h] != (seq_t)0)
        h = (h + 1) & (hash_cap - 1);

    hash_table[h] = key;
    ++hash_cnt;
}

/* ------- Dynamic array of chosen cubes ------- */
static seq_t *cubes   = NULL;
static size_t cube_cnt = 0;
static size_t cube_cap = 0;

static void init_cubes(void)
{
    cube_cap = 8;
    cubes = (seq_t *)calloc(cube_cap, sizeof(seq_t));
}

static void cubes_append(seq_t v)
{
    if (cube_cnt == cube_cap) {
        cube_cap = (cube_cap == 0) ? 8 : cube_cap << 1;
        cubes = (seq_t *)realloc(cubes, cube_cap * sizeof(seq_t));
    }
    cubes[cube_cnt++] = v;
}

/* ------- The required function -------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* offset is 1, but guard against 0 */

    /* initialisation of static state */
    if (hash_table == NULL)
        init_hash();
    if (cubes == NULL)
        init_cubes();

    /* keep track of the next integer root whose cube we test */
    static idx_t next_root = 1;

    /* continue building until we have at least n terms */
    while (cube_cnt < (size_t)n) {

        seq_t candidate = (seq_t)next_root * (seq_t)next_root * (seq_t)next_root;
        bool ok = true;

        /* test against all previously chosen cubes */
        for (size_t i = 0; i < cube_cnt; ++i) {
            seq_t s = cubes[i] + candidate;
            if (hash_contains(s)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            /* accept this cube */
            cubes_append(candidate);

            /* insert all new sums into the hash table */
            for (size_t i = 0; i < cube_cnt - 1; ++i) {
                seq_t s = cubes[i] + candidate;
                hash_insert(s);
            }
            ++next_root;
        } else {
            /* reject, try the next integer root */
            ++next_root;
        }
    }

    return cubes[n - 1];
}
