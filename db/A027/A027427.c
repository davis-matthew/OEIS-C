/*
 * A027427  Number of distinct products ij with 0 <= i < j <= n.
 * https://oeis.org/A027427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct products i*j with 0 <= i < j <= n (0
 *            counts as one distinct product). The implementation
 *            incrementally builds a bitset of all products seen so far,
 *            updates a memoised array, and returns a(n). It runs in O(n²)
 *            time but uses only O(n²) bits of memory, which is well under
 *            t...
 * Range:     u8 n <= 28, u16 n <= 504, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Static state ----------------------------------------------------- */
static seq_t *a_vals   = NULL;     /* memoised a(n) values      */
static size_t a_len   = 0;         /* current length of a_vals  */
static uint64_t *bits = NULL;      /* bitset for seen products  */
static size_t  bits_len = 0;       /* number of 64‑bit words in bits */
static idx_t max_n   = 0;          /* largest n processed       */
static seq_t pos_cnt = 0;          /* distinct positive products */

/* Ensure a_vals can hold at least 'needed' indices */
static void ensure_a_vals(size_t needed)
{
    if (a_len >= needed) return;
    size_t new_len = a_len ? a_len * 2 : 1;
    if (new_len < needed) new_len = needed;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_len * sizeof(seq_t));
    if (!tmp) abort();
    a_vals = tmp;
    a_len = new_len;
    /* initialise any new entries to 0 */
    for (size_t i = a_len - new_len; i < new_len; ++i) a_vals[i] = 0;
}

/* Ensure the bitset covers all products up to 'max_prod' */
static void ensure_bits_size(idx_t max_prod)
{
    size_t needed = (size_t)((max_prod >> 6) + 1);   /* +1 for safety    */
    if (bits_len >= needed) return;
    uint64_t *new_bits = (uint64_t *)realloc(bits,
                                            needed * sizeof(uint64_t));
    if (!new_bits) abort();
    /* zero newly allocated words */
    for (size_t w = bits_len; w < needed; ++w) new_bits[w] = 0ULL;
    bits      = new_bits;
    bits_len  = needed;
}

/* Test whether bit for 'idx' is set; if not, set it and return 0,
   otherwise return 1. */
static int test_and_set(idx_t idx)
{
    size_t w = (size_t)(idx >> 6);
    uint64_t mask = 1ULL << (idx & 63);
    if (bits[w] & mask) return 1;
    bits[w] |= mask;
    return 0;
}

/* Main function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Prepare storage for up to n */
    if (a_len <= (size_t)n) ensure_a_vals((size_t)n + 1);

    /* If already computed up to n, return memoised value */
    if (max_n >= n) return a_vals[n];

    /* Extend computation incrementally from max_n+1 to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* Grow bitset to accommodate products up to m*(m-1) */
        idx_t max_prod = (idx_t)m * (m - 1);
        ensure_bits_size(max_prod);

        for (idx_t i = 1; i < m; ++i) {
            idx_t prod = (idx_t)((seq_t)i * (seq_t)m);
            if (!test_and_set(prod)) ++pos_cnt;
        }
        a_vals[m] = pos_cnt + 1;            /* +1 for the distinct product 0 */
    }

    max_n = n;
    return a_vals[n];
}
