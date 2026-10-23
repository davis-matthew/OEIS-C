/*
 * A116883  A number k is included iff (highest odd divisor of k)^2 >= k.
 * https://oeis.org/A116883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth positive integer k such that (highest odd
 *            divisor of k)^2 >= k
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;

    for (;; ++k) {
        seq_t m = k;
        /* remove all factors of 2 from k to get its highest odd divisor */
        while ((m & 1) == 0)
            m >>= 1;

        if (m * m >= k) {
            ++count;
            if (count == n)
                return k;
        }
    }
}
