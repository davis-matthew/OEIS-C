/*
 * A185634  Number of n-length cycles from any point in a complete graph on n
 *          nodes.
 * https://oeis.org/A185634
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-1)^n + (-1)^n*(n-1))/n (offset 2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Return the number of n‑length cycles from a point in a complete graph on n nodes.
 * Uses the closed form a(n) = ((n-1)^n + (-1)^n*(n-1))/n.
 * For small seq_t types (≤64 bits) a 128‑bit intermediate is used to avoid
 * overflow before the final division.  For large or arbitrary‑precision
 * seq_t values the provided big_pow helper is used. */
seq_t a(idx_t n)
{
    assert(n >= 2);          /* offset starts at 2 */

    /* ------------------------------------------------------------ */
    /* 1.  64‑bit or smaller seq_t: use __uint128_t intermediate   */
    /* ------------------------------------------------------------ */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        uint64_t base = (uint64_t)(n - 1);        /* n-1, fits in 64‑bit */
        __uint128_t pow = 1;                     /* 128‑bit accumulator */

        /* Compute (n-1)^n with 128‑bit precision */
        for (idx_t i = 0; i < n; ++i)
            pow *= base;

        /* Numerator: (n-1)^n + (-1)^n*(n-1) */
        __uint128_t numer = (n & 1ULL)           /* odd n → subtract   */
                           ? (pow - base)
                           : (pow + base);

        /* Final division by n, result fits in seq_t by caller's invariants */
        return (seq_t)(numer / n);
    }

    /* ------------------------------------------------------------ */
    /* 2.  Arbitrary‑precision or 128‑bit seq_t: use big_pow helper */
    /* ------------------------------------------------------------ */
    seq_t m = (seq_t)(n - 1);                     /* base m = n-1 as seq_t */
    seq_t pow = big_pow(m, (seq_t)n);             /* compute m^n as seq_t */

    /* Numerator: (n-1)^n + (-1)^n*(n-1) */
    seq_t numer = (n & 1ULL)                       /* odd n → subtract   */
                   ? (pow - m)
                   : (pow + m);

    /* Final division by n */
    return numer / (seq_t)n;
}
