/*
 * A091954  Number of odd proper divisors of n. That is, the number of odd
 *          divisors of n that are less than n.
 * https://oeis.org/A091954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (number of odd divisors of n) - [n is odd]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t odd_divisor_count(idx_t n) {
    /* Count divisors of the odd part of n */
    while ((n & 1) == 0) n >>= 1;  /* remove all factors of 2 */
    seq_t count = 1;
    idx_t i = 3;
    while ((i * (seq_t)i) <= n) {          /* use seq_t to avoid overflow */
        if (n % i == 0) {
            /* count exponent */
            seq_t e = 1;
            n /= i;
            while (n % i == 0) {
                n /= i;
                e += 1;
            }
            count *= (e + 1);
        }
        i += 2;
    }
    if (n > 1) count *= 2;  /* remaining prime factor */
    return count;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of sequence range, arbitrary */
    seq_t odd_cnt = odd_divisor_count(n);
    if ((n & 1) != 0) odd_cnt -= 1;  /* exclude n itself when odd */
    return odd_cnt;
}
