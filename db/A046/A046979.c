/*
 * A046979  Denominators of Taylor series for exp(x)*sin(x).
 * https://oeis.org/A046979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(4n) = 1 a(n) = n! / 2^floor(n/2) for n not a multiple of 4
 *            (denominators of the Taylor series of exp(x)*sin(x) )
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is a multiple of 4, the coefficient vanishes; return 1 */
    if ((n & 3) == 0)
        return (seq_t)1;

    /* Compute n! / 2^floor(n/2) while cancelling the 2‑factors
       as they appear to keep the intermediate values small.  */
    idx_t twos_needed = n >> 1;              /* floor(n/2) */
    seq_t res = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t k = i;

        /* Remove factor of 2 from k, cancelling with the required
           denominator 2^floor(n/2) as far as possible.  */
        while ((k & 1) == 0) {
            k >>= 1;
            if (twos_needed > 0)
                --twos_needed;
            else
                res *= (seq_t)2;   /* multiply by the remaining 2 */
        }

        /* Multiply by the odd part of i */
        res *= (seq_t)k;
    }
    return res;
}
