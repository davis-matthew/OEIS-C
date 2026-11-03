/*
 * A009001  Expansion of e.g.f: (1+x)*cos(x).
 * https://oeis.org/A009001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = (-1)^(n/2) if n even, n*(-1)^((n-1)/2) if n odd
 * Range:     i8 n <= 128, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 102 terms through n = 9837 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine sign: 1 if floor(n/2) even, -1 otherwise. */
    seq_t sign = (((n / 2) & 1ULL) ? (seq_t)-1 : (seq_t)1);

    /* Even n: result is ±1, odd n: result is ±n. */
    if ((n & 1ULL) == 0)
        return sign;
    else
        return sign * (seq_t)n;
}
