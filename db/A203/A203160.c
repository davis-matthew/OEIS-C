/*
 * A203160  (n-1)-st elementary symmetric function of the first n terms of
 *          (2,3,1,2,3,1,2,3,1,...)=A010882.
 * https://oeis.org/A203160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (product of the first n terms of the 3‑periodic *
 *            sequence 2,3,1) * (sum of their reciprocals). This simplifies to
 *            * a(n) = 6^{⌊n/3⌋-1} * r * (11*⌊n/3⌋ + w) where r and w depend
 *            on * n mod 3 (r = 1,2,6 and w = 0,3,5 respectively)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    /* k = number of complete 3‑blocks in the first n terms.  r and w
     * are chosen according to the remainder rem = n mod 3. */
    idx_t k   = n / 3;      /* number of full periods */
    idx_t rem = n % 3;      /* remainder in the last partial period */

    /* Compute 6^(k-1) iteratively – k ≥ 1 here. */
    seq_t pow6 = (seq_t)1;
    for (idx_t i = 1; i < k; ++i) pow6 *= (seq_t)6;

    /* Determine r and w for the remainder part. */
    seq_t r, w;
    switch (rem) {
        case 0: r = (seq_t)1; w = (seq_t)0; break;
        case 1: r = (seq_t)2; w = (seq_t)3; break;
        case 2: r = (seq_t)6; w = (seq_t)5; break;
        default: r = (seq_t)1; w = (seq_t)0; /* unreachable */
    }

    /* Compute the linear factor (11*k + w). */
    seq_t factor = (seq_t)k * (seq_t)11 + w;

    /* Final result: 6^(k-1) * r * (11*k + w). */
    return pow6 * r * factor;
}
