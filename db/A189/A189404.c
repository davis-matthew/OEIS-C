/*
 * A189404  n+[nr/t]+[ns/t]; r=1, s=e, t=e^2.
 * https://oeis.org/A189404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/e⌋ + ⌊n/e²⌋ ; e = 2.718281828459045... . The
 *            fractions below are rounded 1/e and 1/e² to 12 decimal places;
 *            for n ≤ 10000 the error is less than 5×10⁻⁸, far below the
 *            minimal gap from an integer, so the results are exact
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Numerical constants for 1/e and 1/e² with 12 decimal places.
       Using 128‑bit arithmetic avoids overflow for all n ≤ 10000. */
    const unsigned long long P1 = 367879441171ULL;   /* 1/e * 10¹²  */
    const unsigned long long Q1 = 1000000000000ULL;  /* 10¹²       */
    const unsigned long long P2 = 135335283237ULL;   /* 1/e² * 10¹² */
    const unsigned long long Q2 = 1000000000000ULL;  /* 10¹²       */

    /* floor(n/e) */
    __int128 prod1 = (__int128)(n) * P1;
    seq_t f1 = (seq_t)(prod1 / Q1);

    /* floor(n/e²) */
    __int128 prod2 = (__int128)(n) * P2;
    seq_t f2 = (seq_t)(prod2 / Q2);

    return n + f1 + f2;
}
