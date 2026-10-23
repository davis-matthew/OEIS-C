/*
 * A182447  a(n) = Sum_{k = 0..n} C(n,k)^10.
 * https://oeis.org/A182447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} C(n,k)^10
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t binom = 1;          /* binom = C(n,0) */
    idx_t half  = n / 2;

    for (idx_t k = 0; k <= half; ++k) {
        /* binom^10 */
        seq_t p = 1;
        for (int e = 0; e < 10; ++e) {
            p *= binom;
        }

        if (k == half && (n % 2 == 0)) {
            sum += p;           /* middle term appears once when n is even */
        } else {
            sum += p + p;       /* double for symmetric pairs */
        }

        /* prepare next binom if any */
        if (k < half) {
            binom = binom * ( (seq_t)(n - k) ) / ( (seq_t)(k + 1) );
        }
    }

    return sum;
}
