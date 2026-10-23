/*
 * A127933  Number of halving and tripling steps to reach 1 from the n-th pure
 *          hailstone number in the '3x+1' problem.
 * https://oeis.org/A127933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of halving/tripling steps needed to reach 1 from
 *            the n‑th pure hailstone number (A061641)
 * Range:     u8 n <= 2672, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compile with C11 or C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Simple open-addressing hash for uint64_t ---------- */
static uint64_t *hash_table   = NULL;
static size_t   hash_size     = 0;
static size_t   hash_used     = 0;
#define HASH_LOAD_FACTOR_NUM   7
#define HASH_LOAD_FACTOR_DEN   10

static void hash_rehash(void)
{
    size_t new_size = hash_size << 1;
    uint64_t *new_table = (uint64_t *)calloc(new_size, sizeof(uint64_t));
    if (!new_table) exit(EXIT_FAILURE);

    size_t mask = new_size - 1;
    for (size_t i = 0; i < hash_size; ++i) {
        uint64_t k = hash_table[i];
        if (!k) continue;
        size_t idx = k & mask;
        while (new_table[idx]) idx = (idx + 1) & mask;
        new_table[idx] = k;
    }
    free((void *)hash_table);
    hash_table = new_table;
    hash_size  = new_size;
}

static void hash_put(uint64_t key)
{
    if (!key) return;  /* 0 is never inserted */
    if (hash_used * HASH_LOAD_FACTOR_DEN > hash_size * HASH_LOAD_FACTOR_NUM)
        hash_rehash();

    size_t mask = hash_size - 1;
    size_t idx = key & mask;
    while (hash_table[idx]) {
        if (hash_table[idx] == key) return;   /* already present */
        idx = (idx + 1) & mask;
    }
    hash_table[idx] = key;
    ++hash_used;
}

static bool hash_has(uint64_t key)
{
    if (!key) return false;
    size_t mask = hash_size - 1;
    size_t idx = key & mask;
    while (hash_table[idx]) {
        if (hash_table[idx] == key) return true;
        idx = (idx + 1) & mask;
    }
    return false;
}

/* ---------- Storage for pure numbers and path buffer ---------- */
static seq_t  *stopping   = NULL;
static size_t   st_len    = 0;
static size_t   st_cap    = 0;

static uint64_t *path_buf  = NULL;
static size_t   path_cap   = 0;

/* ---------- One-time initialization ---------- */
static void ensure_init(void)
{
    if (!hash_table) {
        hash_size = 1 << 20;                     /* about 1M slots */
        hash_table = (uint64_t *)calloc(hash_size, sizeof(uint64_t));
        if (!hash_table) exit(EXIT_FAILURE);
    }
    if (!stopping) {
        st_cap = 256;
        stopping = (seq_t *)malloc(st_cap * sizeof(seq_t));
        if (!stopping) exit(EXIT_FAILURE);
    }
    if (!path_buf) {
        path_cap = 128;
        path_buf = (uint64_t *)malloc(path_cap * sizeof(uint64_t));
        if (!path_buf) exit(EXIT_FAILURE);
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static idx_t current_candidate = 1;  /* next number to test */

    ensure_init();

    while (st_len < n) {
        uint64_t candidate = current_candidate++;
        bool impure = false;

        /* Check if candidate already appeared in any earlier trajectory */
        if (hash_has(candidate))
            impure = true;

        /* Generate entire Collatz trajectory for the candidate */
        size_t path_len = 0;
        uint64_t current = candidate;
        uint64_t steps   = 0;

        while (true) {
            /* Grow path buffer if necessary */
            if (path_len == path_cap) {
                path_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(path_buf,
                                                    path_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                path_buf = tmp;
            }

            path_buf[path_len++] = current;
            hash_put(current);           /* mark number as seen */

            if (current == 1) break;     /* reached stopping point */

            /* Apply the 3x+1 rule */
            if ((current & 1) == 0)
                current >>= 1;
            else
                current = 3 * current + 1;

            ++steps;
        }

        /* Store the step count for pure candidates.
           Adjust for the special case of the start value 2 (see OEIS). */
        if (!impure) {
            seq_t val = (seq_t)steps;
            if (candidate <= 2)
                val = (seq_t)0;  /* a(1) and a(2) are defined as 0 */
            if (st_len == st_cap) {
                st_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(stopping,
                                              st_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                stopping = tmp;
            }
            stopping[st_len++] = val;
        }
    }

    return stopping[n - 1];
}
