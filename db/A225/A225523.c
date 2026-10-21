/*
 * A225523  Number of distinct products i*j over all pairs (i,j) with |i| +
 *          |j| <= n.
 * https://oeis.org/A225523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2 * |{ a*b : a,b >= 1, a+b <= n }|
 * Range:     u8 n <= 29, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
/* The function assumes that seq_t and idx_t are defined elsewhere. */

seq_t a(idx_t n)
{
    /* Edge cases: for n = 0 or 1, only product 0 exists. */
    if (n <= 1) {
        return (seq_t)1;           /* 0 is the only product, counted once. */
    }

    /* Maximum possible product for given n: floor(n/2)*ceil(n/2). */
    idx_t max_prod = (n / 2) * (n - n / 2);   /* (n/2) is floor division. */

    /* Allocate a bitset of (max_prod+1) bits, word size 64 bits. */
    idx_t bits_needed = max_prod + 1;
    idx_t word_cnt    = (bits_needed + 63) / 64;   /* number of 64‑bit words */

    uint64_t *bits = (uint64_t *)malloc((size_t)word_cnt * sizeof(uint64_t));
    if (!bits) return (seq_t)0;  /* fallback in case of allocation failure */

    /* Initialize bitset to zero. */
    for (idx_t w = 0; w < word_cnt; ++w) {
        bits[w] = 0ull;
    }

    seq_t unique = 0;    /* number of distinct positive products */

    /* Enumerate all pairs (a,b) with a,b >= 1 and a+b <= n. */
    for (idx_t a = 1; a < n; ++a) {
        for (idx_t b = 1; a + b <= n; ++b) {
            idx_t prod = a * b;         /* product fits in idx_t (n <= 10000). */
            idx_t w    = prod >> 6;      /* prod / 64  */
            idx_t bit  = prod & 63ull;   /* prod % 64  */
            uint64_t mask = 1ull << bit;
            if (!(bits[w] & mask)) {    /* new product encountered */
                bits[w] |= mask;
                ++unique;               /* count distinct products */
            }
        }
    }

    free(bits);

    /* Final answer: 1 zero + 2 for each distinct positive product. */
    seq_t result = (seq_t)1;
    seq_t tmp    = (seq_t)2 * unique;
    return result + tmp;
}
