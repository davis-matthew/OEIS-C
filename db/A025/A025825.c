/*
 * A025825  Expansion of 1/((1-x^2)*(1-x^9)*(1-x^12)).
 * https://oeis.org/A025825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 19*n +369 + 4*n*((n+2) mod 3) +
 *            9*(n+7)*(-1)^n)/432)
 * Range:     u8 none, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre-calculate modulo and sign components */
    idx_t n_mod3 = (n + 2) % 3;            /* ((n+2) mod 3) */
    seq_t term1 = (seq_t)n * (seq_t)n;      /* n^2 */
    seq_t term2 = 19 * (seq_t)n;            /* 19*n */
    seq_t term3 = 369;                      /* constant term */
    seq_t term4 = 4 * (seq_t)n * (seq_t)n_mod3; /* 4*n*((n+2) mod 3) */
    seq_t term5 = 9 * ((seq_t)n + 7);       /* 9*(n+7) */

    /* (-1)^n as +1 or -1 using signed long long */
    long long sign = ((n & 1) ? -1LL : 1LL);

    /* Compute numerator in signed 64‑bit to allow subtraction */
    long long numerator =
        (long long)term1 + (long long)term2 + (long long)term3
        + (long long)term4 + sign * (long long)term5;

    /* Division by 432 yields the desired floor value */
    seq_t result = (seq_t)(numerator / 432);

    return result;
}
