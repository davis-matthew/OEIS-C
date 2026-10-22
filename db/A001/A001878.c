/*
 * A001878  Number of divisors of n of the form 5k+3; a(0) = 0.
 * https://oeis.org/A001878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n of the form 5k+3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    
    for (idx_t d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            if (d % 5 == 3) {
                count++;
            }
            idx_t other = n / d;
            if (d != other && other % 5 == 3) {
                count++;
            }
        }
    }
    
    return count;
}
