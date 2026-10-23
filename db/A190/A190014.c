/*
 * A190014  a(0)=0, a(1)=1, if n = (n-1)', then a(n)=0 otherwise
 *          a(n)=2*a((n-1)'), where n' is the arithmetic derivative of n.
 * https://oeis.org/A190014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1. For n ≥ 2: let d = (n-1)' be the arithmetic
 *            derivative of (n‑1). If n == d then a(n) = 0, otherwise a(n) = 2
 *            * a(d)
 * Range:     u8 n <= 54, u16 n <= 2336, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   arith_derivative
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/arith_derivative.h"

/* --- simple hash table for memoisation ------------------------------------- */
struct Entry {
    idx_t key;   /* 0 is used as an empty marker; we never cache n = 0 */
    seq_t value;
};

static struct Entry *hash_table = NULL;
static size_t hash_capacity = 0;
static size_t hash_size = 0;

static void hash_init(void)
{
    if (hash_table) return;
    hash_capacity = 131072; /* 2^17, power of two for masking */
    hash_table = (struct Entry *)calloc(hash_capacity, sizeof(struct Entry));
}

static struct Entry *hash_find(idx_t key)
{
    if (!hash_table) return NULL;
    size_t idx = key & (hash_capacity - 1);
    while (hash_table[idx].key != 0) {
        if (hash_table[idx].key == key) return &hash_table[idx];
        idx = (idx + 1) & (hash_capacity - 1);
    }
    return NULL;
}

static struct Entry *hash_insert(idx_t key, seq_t value)
{
    if (!hash_table) hash_init();
    size_t idx = key & (hash_capacity - 1);
    while (hash_table[idx].key != 0) {
        if (hash_table[idx].key == key) {
            hash_table[idx].value = value;
            return &hash_table[idx];
        }
        idx = (idx + 1) & (hash_capacity - 1);
    }
    hash_table[idx].key = key;
    hash_table[idx].value = value;
    ++hash_size;

    /* rehash if load factor exceeds 0.7 */
    if (hash_size > hash_capacity * 7 / 10) {
        size_t new_capacity = hash_capacity << 1;
        struct Entry *new_table = (struct Entry *)calloc(new_capacity, sizeof(struct Entry));
        for (size_t i = 0; i < hash_capacity; ++i) {
            if (hash_table[i].key != 0) {
                size_t j = hash_table[i].key & (new_capacity - 1);
                while (new_table[j].key != 0) j = (j + 1) & (new_capacity - 1);
                new_table[j] = hash_table[i];
            }
        }
        free(hash_table);
        hash_table = new_table;
        hash_capacity = new_capacity;
    }
    return &hash_table[idx];
}

/* --- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    hash_init();
    struct Entry *e = hash_find(n);
    if (e) return e->value;

    idx_t m = n - 1;
    seq_t d = arith_derivative(m);
    seq_t result;
    if ((seq_t)n == d) {
        result = 0;
    } else {
        seq_t sub = a((idx_t)d);
        result = sub << 1;  /* multiply by 2 */
    }
    hash_insert(n, result);
    return result;
}
