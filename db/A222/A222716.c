/*
 * A222716  Numbers which are both the sum of n+1 consecutive triangular
 *          numbers and the sum of the n-1 immediately following triangular
 *          numbers.
 * https://oeis.org/A222716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(n+1)*(3*n^2-2)/6
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 97, u64 n <= 8192,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A222716.
 * Uses an algebraic formula that avoids large intermediate values
 * even for 64‑bit builds by dividing the 6 early. */
seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) – behavior for n==0 is irrelevant */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;      /* original n in seq_t type               */
    seq_t p1 = N - 1;        /* factor (n-1)                           */
    seq_t p2 = N;            /* factor  n                              */
    seq_t p3 = N + 1;        /* factor (n+1)                           */

    /* -------------------------------------------------------------
     *  The product (n‑1)*n*(n+1) is divisible by 6.
     *  We divide by 2 and 3 before the final multiplication
     *  to keep intermediate values small (essential for 64‑bit).
     * ------------------------------------------------------------- */

    /* divide by 2 */
    if ((p1 & 1) == 0)
        p1 >>= 1;
    else if ((p2 & 1) == 0)
        p2 >>= 1;
    else
        p3 >>= 1;        /* p3 must be even */

    /* divide by 3 */
    if (p1 % 3 == 0)
        p1 /= 3;
    else if (p2 % 3 == 0)
        p2 /= 3;
    else
        p3 /= 3;        /* p3 must be divisible by 3 */

    /* multiply the reduced factors */
    seq_t t = p1 * p2;
    t *= p3;            /* t = ((n-1)*n*(n+1))/6  */

    /* compute the remaining factor: 3*n^2 - 2 */
    seq_t d = (seq_t)3 * N * N - 2;

    /* final result */
    t *= d;
    return t;
}
