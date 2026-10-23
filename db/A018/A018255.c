/*
 * A018255  Divisors of 30.
 * https://oeis.org/A018255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 30 in increasing order (0 for n > 8)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t divisors[8];
    static int initialized = 0;

    if (!initialized) {
        /* Generate the divisors of 30 in ascending order. */
        idx_t count = 0;
        for (idx_t d = 1; d <= 30; ++d) {
            if (30 % d == 0) {
                divisors[count++] = (seq_t)d;
            }
        }
        initialized = 1;
    }

    if (n == 0 || n > 8) {
        return (seq_t)0;
    }
    return divisors[n - 1];
}
