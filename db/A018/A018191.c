/*
 * A018191  a(n) = Sum_{k=0..n} binomial(n, k) * k! / floor(k/2)!.
 * https://oeis.org/A018191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * k! / floor(k/2)! Computed by
 *            iterating the terms T(k) = n! / ((n-k)! * floor(k/2)!) using the
 *            recurrence: T(0) = 1 T(k+1) = T(k) * (n-k) if k is even T(k+1) =
 *            T(k) * (n-k) / ((k+1)/2) if k is odd The partial sum of these
 *            terms gives a(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t term = (seq_t)1;   /* T(0) */
    seq_t sum  = term;       /* sum over k=0 */

    for (idx_t k = 0; k < n; ++k) {
        /* multiply by (n-k) */
        term = term * ((seq_t)(n - k));

        /* if k is odd, divide by ((k+1)/2) */
        if (k & 1U) {
            idx_t divisor = (k + 1) / 2;   /* guarantees integer division */
            term = term / ((seq_t)divisor);
        }

        sum += term;
    }

    return sum;
}
