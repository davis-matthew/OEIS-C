/*
 * A020343  Expansion of 1/((1-5x)(1-8x)(1-9x)).
 * https://oeis.org/A020343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 157*a(n-2) + 360*a(n-3) for n > 2, with a(0)
 *            = 1, a(1) = 22, a(2) = 327
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)22;
    if (n == 2u) return (seq_t)327;

    /* Use a sliding window of the last three terms. */
    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)22;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)327; /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* next = 22*a_nm1 - 157*a_nm2 + 360*a_nm3 */
        seq_t term1 = a_nm1 * (seq_t)22;
        seq_t term2 = a_nm2 * (seq_t)157;
        seq_t term3 = a_nm3 * (seq_t)360;
        a_n = term1 + term3 - term2;

        /* shift window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
