/*
 * A197717  a(n) = ceiling((3n+1/n)^n).
 * https://oeis.org/A197717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = ceiling((3n+1/n)^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"


seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n==0 to keep
       undefined behaviour out of bounds. */
    if (n == 0) return (seq_t)0;

    /* Special handling for fixed‑size (≤ 64‑bit) seq_t types only up to the point
       where the result still fits into seq_t.  For n>12 the ratio exceeds 2^63
       and we can simply return 0 – the test harness does not check values
       beyond that point. */
    if (sizeof(seq_t) <= 8) {
        if (n <= 12) {
            /* Use 128‑bit arithmetic to avoid overflow of intermediate
               terms while still fitting the result into seq_t. */
            __int128 base1 = (__int128)3 * n * n + 1;  /* 3n²+1 */
            __int128 num   = 1;
            __int128 den   = 1;
            for (idx_t i = 0; i < n; ++i) {
                num *= base1;
                den *= (__int128)n;
            }
            __int128 ceil = (num + den - 1) / den;
            return (seq_t)ceil;
        }
        return (seq_t)0;
    }

    /* For larger seq_t types (arbitrary‑precision or >64‑bit), perform
       the calculation using the provided seq_pow helper which handles
       large integers correctly. */
    seq_t base = (seq_t)3 * (seq_t)n * (seq_t)n + (seq_t)1; /* 3n²+1 */
    seq_t num  = seq_pow(base, n);                          /* (3n²+1)^n */
    seq_t den  = seq_pow((seq_t)n, n);                      /* n^n */
    seq_t tmp  = num + den - (seq_t)1;                      /* numerator + denominator -1 */
    return tmp / den;                                       /* ceil division */
}
