/*
 * A228091  Numbers n for which there exists such a natural number k < n that
 *          k + bitcount(k) = n + bitcount(n), where bitcount(k) (A000120)
 *          gives the number of 1's in binary representation of nonnegative
 *          integer k.
 * https://oeis.org/A228091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A228091: Numbers n for which there exists k < n with
   k + popcount(k) = n + popcount(n).  This implementation
   generates terms on demand by scanning integers and recording
   the first occurrence of each value of f(x) = x + popcount(x).
   Subsequent integers producing a duplicate f form the sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* popcount for idx_t (uint64_t) using compiler intrinsic when available */
static inline unsigned popcount_idx(idx_t v)
{
#ifdef __GNUC__
    return __builtin_popcountll((unsigned long long)v);
#else
    unsigned cnt = 0;
    while (v) { cnt += v & 1U; v >>= 1; }
    return cnt;
#endif
}

/* popcount for seq_t (works for any unsigned integral type or
   big integer class that supports &, >>, + and bool conversion) */
static inline seq_t popcount_seq(seq_t x)
{
    seq_t cnt = 0;
    seq_t one = 1;
    while (x) {
        cnt += x & one;
        x >>= 1;
    }
    return cnt;
}

/*---------------------------------------------------------------*/
/* Simple open-addressing hash table for seq_t keys               */
/*---------------------------------------------------------------*/
static seq_t *hash_keys = NULL;
static unsigned char *hash_used = NULL;
static size_t hash_capacity = 0;
static size_t hash_mask = 0;

static void init_hash(void)
{
    if (hash_keys != NULL) return;
    hash_capacity = 1U << 20;          /* 1 048 576 slots */
    hash_mask = hash_capacity - 1U;
    hash_keys = (seq_t *)calloc(hash_capacity, sizeof(seq_t));
    hash_used = (unsigned char *)calloc(hash_capacity, sizeof(unsigned char));
}

/* simple 64‑bit hash derived from the bits of the key */
static inline uint64_t hash_key(seq_t k)
{
    uint64_t h = 0;
    seq_t tmp = k;
    while (tmp) {
        h ^= (uint64_t)(tmp & 0xFFFFFFFFULL);
        tmp >>= 32;
    }
    return h;
}

static bool hash_insert_or_check(seq_t key)
{
    init_hash();
    uint64_t h = hash_key(key);
    size_t idx = h & hash_mask;

    while (hash_used[idx]) {
        if (hash_keys[idx] == key) return true;   /* duplicate found */
        idx = (idx + 1) & hash_mask;
    }

    /* new key */
    hash_used[idx] = 1;
    hash_keys[idx] = key;
    return false;
}

/*---------------------------------------------------------------*/
/* Sequence storage                                                */
/*---------------------------------------------------------------*/
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static idx_t next_integer = 1;   /* next integer to examine for f(i) */

static void grow_seq(size_t needed)
{
    if (seq_len >= needed) return;
    if (seq_cap == 0) {
        seq_cap = 8;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }
    while (seq_cap < needed) {
        seq_cap <<= 1;
        seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
    }
}

/*---------------------------------------------------------------*/
/* Sequence function                                               */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* offset is 1 – undefined below */

    if (seq_len < (size_t)n) {
        grow_seq((size_t)n);

        idx_t cur = next_integer;
        while (seq_len < (size_t)n) {
            seq_t f = (seq_t)cur + (seq_t)popcount_idx(cur);
            if (hash_insert_or_check(f)) {
                seq_vals[seq_len++] = (seq_t)cur;
            }
            ++cur;
        }
        next_integer = cur;             /* start next search from here */
    }

    return seq_vals[n - 1];
}
