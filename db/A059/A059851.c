/*
 * A059851  a(n) = n - floor(n/2) + floor(n/3) - floor(n/4) + ... (this is a
 *          finite sum).
 * https://oeis.org/A059851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)
 * Range:     u8 n <= 358, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t posSum = 0;   /* sum of floor(n/k) for odd k */
    seq_t negSum = 0;   /* sum of floor(n/k) for even k */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = (seq_t)(n / k);   /* floor(n/k) as a seq_t */
        if ((k & 1) == 0) {            /* even k → subtract */
            negSum += term;
        } else {                       /* odd k  → add */
            posSum += term;
        }
    }
    return posSum - negSum;
}
