/*
 * A191093  [Squarefree part of (ABC)]/C for A=2, C=A+B, as a function of B,
 *          rounded to nearest integer.
 * https://oeis.org/A191093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round( rad(2*n*(n+2)) / (n+2) ), where rad(x) is the
 *            product of distinct prime factors of x
 * Range:     u8 n <= 10, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void factor_and_mul(idx_t m, seq_t *sqp)
{
    /* Multiply each distinct prime factor of m (except 2) into *sqp. */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            if (p != 2) {
                *sqp *= (seq_t)p;
            }
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    if (m > 1 && m != 2) {
        *sqp *= (seq_t)m;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset handling */

    idx_t B = n;
    idx_t C = B + 2;

    seq_t sqp = (seq_t)2;          /* prime 2 always contributes */
    factor_and_mul(B, &sqp);       /* primes of B (excluding 2) */
    factor_and_mul(C, &sqp);       /* primes of C (excluding 2) */

    /* round to nearest integer: (sqp + C/2) / C */
    return (sqp + (seq_t)(C / 2)) / (seq_t)C;
}
