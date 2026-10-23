/*
 * A129344  a(n) is the number of powers of 2 that have n decimal digits.
 * https://oeis.org/A129344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129344: number of powers of 2 with exactly n decimal digits.
   Formula for n > 1:  a(n) = ⌊n·L⌋ – ⌊(n‑1)·L⌋,  where L = log₂(10).
   Special case for n = 1: a(1) = 4 (contains 2⁰, 2¹, 2², 2³). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the special case n = 1 */
    if (n == 1)
        return (seq_t)4;

    /* Rational approximation L ≈ NUM / DEN with error < 1e‑18.
       NUM and DEN are chosen so that floor(n*L) is exact for
       all n up to 10 000. */
    const unsigned __int128 NUM = (unsigned __int128)3321928094887362347ULL;
    const unsigned __int128 DEN = (unsigned __int128)1000000000000000000ULL;

    /* Compute floor(n*L) and floor((n‑1)*L) using integer arithmetic. */
    unsigned __int128 prod_n   = (unsigned __int128)n * NUM;
    unsigned __int128 floor_n  = prod_n / DEN;

    unsigned __int128 prod_nm1 = (unsigned __int128)(n - 1) * NUM;
    unsigned __int128 floor_nm1 = prod_nm1 / DEN;

    /* The difference fits in seq_t for all n handled. */
    return (seq_t)(floor_n - floor_nm1);
}
