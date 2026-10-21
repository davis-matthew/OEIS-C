/*
 * A225526  Number of distinct products i*j over all pairs (i,j) with |i| +
 *          |j| <= n and gcd(i,j) <= 1.
 * https://oeis.org/A225526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2 * |{ a*b : a>0, b>0, a+b ≤ n, gcd(a,b)=1 }| (i.e. 1
 *            for the zero product plus 2 for each distinct positive product,
 *            one positive and one negative.)
 * Range:     u8 n <= 31, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Euclidean gcd for non‑negative idx_t values. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Zero product (from any pair with one component zero) is always present. */
    if (n == 0) return (seq_t)1;

    /* Max possible product for a+b ≤ n with a,b>0:
       floor(n/2) * ceil(n/2).  This fits in idx_t. */
    idx_t half = n >> 1;                 /* floor(n/2) */
    idx_t max_prod = half * (n - half);  /* floor(n/2) * ceil(n/2) */

    /* Bitset to record which positive products have appeared. */
    idx_t bits = max_prod + 1;                /* include 0..max_prod */
    idx_t bytes = (bits + 7) >> 3;           /* ceil(bits/8) */
    uint8_t *bitset = (uint8_t*)calloc((size_t)bytes, 1);
    if (!bitset) return (seq_t)0;            /* fallback, should not happen */

    idx_t distinct = 0;

    /* Enumerate unordered coprime pairs (a,b) with a,b>0, a+b≤n. */
    for (idx_t a = 1; a <= n; ++a) {
        idx_t max_b = n - a; /* b must satisfy a+b ≤ n */
        if (max_b < a) break;               /* would make pairs ordered wrongly */
        for (idx_t b = a; b <= max_b; ++b) {
            if (gcd_idx(a, b) != 1) continue;

            idx_t prod = a * b;                     /* a,b ≤ n ⇒ prod fits in idx_t */
            idx_t byte = prod >> 3;                 /* prod/8 */
            uint8_t mask = 1u << (prod & 7);        /* prod mod 8 */
            if ((bitset[byte] & mask) == 0) {
                bitset[byte] |= mask;
                ++distinct;
            }
        }
    }

    free(bitset);

    /* 1 for zero product + 2 for each distinct positive product. */
    return (seq_t)1 + (seq_t)2 * distinct;
}
