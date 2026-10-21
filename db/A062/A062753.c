/*
 * A062753  Multiples of 4 whose sum of digits is also a multiple of 4.
 * https://oeis.org/A062753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 4090, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the nth term of the sequence of multiples of 4
 *  whose sum of digits is also a multiple of 4.
 *  The sequence starts 4, 8, 40, 44, 48, ...  (A062753)
 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1, return 0 */

    idx_t count = 0;
    seq_t m = 4;                    /* first multiple of 4 */

    while (1) {
        seq_t sum = 0;
        seq_t t = m;
        while (t > 0) {
            sum += t % 10;
            t /= 10;
        }
        if (sum % 4 == 0) {
            count++;
            if (count == n)
                return m;
        }
        m += 4;                     /* next multiple of 4 */
    }
}
