/*
 * A022263  Gaussian binomial coefficients [ n,12 ] for q = 9.
 * https://oeis.org/A022263
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..12} (9^(n-i+1)-1)/(9^i-1) , OEIS A022263
 *            (Gaussian binomial coefficients [n,12] for q = 9, offset 12)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 13, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp)          */
#include "../utils/seq_gcd.h"   /* seq_gcd(seq_t a, seq_t b)              */

seq_t a(idx_t n)
{
    /* The OEIS offset is 12.  Values for n < 12 are irrelevant. */
    if (n < 12) return 0;

    /* For the small 64‑bit case (builds that use uint64_t) the
       sequence fits in 64 bits only for n = 12 and n = 13.
       Return the known values directly to avoid overflow. */
    if (n == 12) return (seq_t)1U;
    if (n == 13 && sizeof(seq_t) <= sizeof(uint64_t))
        return (seq_t)317733228541ULL;   /* a(13) */

    /* For larger n (or for arbitrary‑precision seq_t) use a
       cancellation strategy that keeps intermediate sizes moderate. */
    static seq_t denom[13];
    static bool expanded = false;
    if (!expanded) {
        for (idx_t i = 1; i <= 12; ++i)
            denom[i] = big_pow((seq_t)9, (seq_t)i) - (seq_t)1;
        expanded = true;
    }

    seq_t result = 1;

    for (idx_t i = 1; i <= 12; ++i) {
        seq_t numer = big_pow((seq_t)9, (seq_t)(n - i + 1)) - (seq_t)1;
        seq_t d     = denom[i];

        /* Reduce the fraction numer/d as much as possible */
        seq_t g = seq_gcd(numer, d);
        numer /= g;
        d     /= g;

        /* Cancel any common factor between the current result and d */
        g = seq_gcd(result, d);
        result /= g;
        d     /= g;

        /* Multiply and divide – division is exact by construction */
        result *= numer;
        result /= d;
    }

    return result;
}
