/*
 * A062854  First differences of A027424.
 * https://oeis.org/A062854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (# distinct products i*j with 1≤i,j≤n) minus that for
 *            n-1. That is, a(n) is the first difference of A027424
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t            max_n         = 0;      /* largest n processed so far     */
static size_t           a_cap          = 0;      /* capacity of a_vals (0..max_n) */
static seq_t           *a_vals         = NULL;   /* a_vals[1..max_n] holds a(k)     */
static size_t           bitset_cap     = 0;      /* number of 64‑bit words used     */
static uint64_t        *bitset         = NULL;   /* bitset of all products seen    */

seq_t a(idx_t n)
{
    if (n == 0U)   /* undefined, but safe return */
        return 0U;

    /* Expand cached data if a larger n is requested */
    if ((uint64_t)n > max_n) {
        /* Resize a_vals array */
        size_t new_cap = (size_t)n + 1;          /* indices 0..n inclusive   */
        seq_t *new_arr = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!new_arr)
            return 0U;                          /* allocation failure     */
        a_vals = new_arr;
        /* zero the new part if any */
        for (size_t i = a_cap; i < new_cap; ++i)
            a_vals[i] = 0U;
        a_cap = new_cap;

        /* Resize bitset to cover products up to n*n */
        uint64_t prod_limit = (uint64_t)n * (uint64_t)n;      /* inclusive     */
        size_t   new_bitset_cap = (size_t)(prod_limit >> 6) + 1;  /* words needed */
        if (new_bitset_cap > bitset_cap) {
            uint64_t *new_bits = (uint64_t *)realloc(bitset,
                                                        new_bitset_cap * sizeof(uint64_t));
            if (!new_bits)
                return 0U;                          /* allocation failure   */
            /* zero newly added words */
            for (size_t i = bitset_cap; i < new_bitset_cap; ++i)
                new_bits[i] = 0ULL;
            bitset = new_bits;
            bitset_cap = new_bitset_cap;
        }

        /* Incrementally compute a(k) for k = max_n+1 .. n */
        for (uint64_t cur = max_n + 1ULL; cur <= (uint64_t)n; ++cur) {
            uint64_t count_new = 0ULL;
            uint64_t cur_n = cur;
            for (uint64_t j = 1ULL; j <= cur_n; ++j) {
                uint64_t prod = cur_n * j;                 /* <= n*n   */
                size_t   word = prod >> 6;                 /* divide by 64 */
                uint64_t mask = 1ULL << (prod & 63ULL);    /* bit position */
                if ((bitset[word] & mask) == 0ULL) {
                    bitset[word] |= mask;                  /* mark seen   */
                    ++count_new;
                }
            }
            a_vals[(size_t)cur] = (seq_t)count_new;        /* store a(cur) */
        }

        max_n = n;   /* update largest processed n */
    }

    return a_vals[(size_t)n];
}
