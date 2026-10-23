/*
 * A011596  Legendre symbol (n,67).
 * https://oeis.org/A011596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Legendre symbol (n, 67). The Legendre symbol is
 * (n|67) = 0 if 67|n, = 1 if n is a quadratic residue mod 67,
 * and = –1 otherwise.  This is computed via Euler's criterion:
 * a(n) = n^33 (mod 67), then interpret the result as 1, –1 or 0.
 */

extern seq_t a(idx_t n)
{
    /* 67 divides n => symbol is 0 */
    if ((n % 67ULL) == 0ULL)
        return 0;

    /* Reduce the index modulo 67 */
    uint64_t m = n % 67ULL;          /* 0 < m < 67 */

    /* Compute m^33 mod 67 by repeated squaring */
    uint64_t res  = 1;
    uint64_t base = m;
    /* 33 is (67‑1)/2, a constant small enough to loop a fixed number */
    for (uint64_t i = 0; i < 33ULL; ++i) {
        res = (res * base) % 67ULL;
    }

    /* Interpretation of the residue: 1, –1 or –1 if not 1 */
    if (res == 1ULL)
        return 1;
    return (seq_t)-1;
}
