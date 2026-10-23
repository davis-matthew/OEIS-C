/*
 * A022277  a(n) = n*(19*n + 1)/2.
 * https://oeis.org/A022277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(19*n + 1)/2
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the calculation type. */
    seq_t sn = (seq_t)n;

    /* Handle the even/odd cases to avoid truncation errors.
       For odd n, 19*n + 1 is even, so we can divide it by 2
       safely before multiplying by n.
       For even n, n itself is divisible by 2, so we divide n
       first and multiply by the full numerator. */
    if (sn & 1U) {                /* n is odd */
        seq_t half = ((sn * (seq_t)19) + (seq_t)1) / (seq_t)2;
        return sn * half;
    } else {                      /* n is even */
        seq_t half = sn / (seq_t)2;
        return half * ((sn * (seq_t)19) + (seq_t)1);
    }
}
