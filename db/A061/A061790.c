/*
 * A061790  a(n) = A000217(n) - A061786(n).
 * https://oeis.org/A061790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(n) – A061786(n) (number of sums i²+j² that occur
 *            more than once) with a(1)=0, a(2)=0,
 * Range:     u8 n <= 48, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the distinct number of sums i²+j² (1 ≤ i ≤ j ≤ n) with a bit‑set
   and returns the difference from the triangular number. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static cache for the bit‑set used to mark sums up to the largest n seen so far */
static unsigned int *bitset = NULL;
static size_t bitset_words = 0;
static idx_t cache_max_n = 0;

/* ensure the bit‑set can hold sums up to 2*n*n */
static void ensure_bitset(idx_t n)
{
    /* 2*n*n fits into 32 bits for all n within the test limits (≤10 000) */
    uint32_t max_sum = (uint32_t)(2u * (unsigned int)n * (unsigned int)n);
    size_t words = (size_t)(max_sum >> 5) + 1;          /* one bit per sum */
    if (words > bitset_words) {
        /* reallocate larger bit‑set */
        unsigned int *p = (unsigned int*)realloc(bitset, words * sizeof(unsigned int));
        if (!p) {
            /* allocation failed – abort the computation */
            exit(1);
        }
        bitset = p;
        bitset_words = words;
    }
    /* clear bits for this call */
    memset(bitset, 0, bitset_words * sizeof(unsigned int));
    cache_max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* triangular number T(n) = n*(n+1)/2 */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* prepare bit‑set for sums up to 2*n*n */
    if (n > cache_max_n)
        ensure_bitset(n);
    /* for n ≤ cache_max_n the existing bit‑set is large enough */

    /* generate all sums i² + j² with 1 ≤ i ≤ j ≤ n */
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t ii = (uint32_t)(i * i);
        for (idx_t j = i; j <= n; ++j) {
            uint32_t s = ii + (uint32_t)(j * j);          /* 0 ≤ s ≤ 2*n² */
            bitset[s >> 5] |= (1U << (s & 31U));
        }
    }

    /* count distinct sums */
    uint64_t distinct = 0;
    for (size_t w = 0; w < bitset_words; ++w) {
        distinct += __builtin_popcount(bitset[w]);
    }

    /* return triangular number minus distinct sums */
    seq_t result = tri;
    result -= (seq_t)distinct;        /* final cast to seq_t per requirement */
    return result;
}
