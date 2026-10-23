/*
 * A116713  Number of permutations of length n which avoid the patterns 123,
 *          2431, 4132.
 * https://oeis.org/A116713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + n^2 - n - 4)/2 for n > 2; a(1) = 1, a(2) = 2, a(3)
 *            = 5 (verified by the recurrence)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* compute 2^(n-1) by repeated doubling */
    seq_t pow2half = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow2half *= (seq_t)2;

    /* compute (n^2 - n - 4)/2 */
    seq_t sn = (seq_t)n;
    seq_t temp = sn * sn;        /* n^2  (no overflow for big‑int types) */
    temp -= sn;                 /* n^2 - n */
    temp -= (seq_t)4;            /* n^2 - n - 4  (non‑negative for n ≥ 3) */
    temp /= (seq_t)2;            /* divide by 2 */

    /* final result */
    return pow2half + temp;
}
