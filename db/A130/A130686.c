/*
 * A130686  Absolute difference of final digits of two consecutive triangular
 *          numbers.
 * https://oeis.org/A130686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = |(n(n+1)/2) mod 10 – ((n‑1)n/2) mod 10|
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses modulo 20 to avoid overflow: (n(n+1) mod 20)/2 gives the last
   decimal digit of the n‑th triangular number.  The sequence uses
   T_0 = 0, so a(1) = |digit(T_1) – digit(T_0)| = 1. */
#include <stdint.h>

static inline seq_t tri_last_digit(idx_t m)
{
    /* Compute the last decimal digit of the triangular number T_m = m(m+1)/2. */
    idx_t a = m % 20;
    idx_t b = (m + 1) % 20;
    /* Product modulo 20 is guaranteed even. */
    idx_t prod_mod20 = (a * b) % 20;
    seq_t digit = (seq_t)(prod_mod20 / 2) % 10;   /* digit in [0,9] */
    return digit;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for safety return 0 when n == 0. */
    if (n == 0)
        return 0;
    seq_t dn  = tri_last_digit(n);      /* digit of T_n   */
    seq_t dnm = tri_last_digit(n - 1);  /* digit of T_{n-1} */
    return dn > dnm ? dn - dnm : dnm - dn;
}
