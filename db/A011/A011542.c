/*
 * A011542  Minimal number of coins needed to pay n cents using coins of sizes
 *          1, 5, 10, 25, 50 cents.
 * https://oeis.org/A011542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊n/50⌋ + ⌊(n%50)/25⌋ + ⌊((n%50)%25)/10⌋ +
 *            ⌊(((n%50)%25)%10)/5⌋ + ((n%5) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 0;
    res += (seq_t)(n / 50);
    n  %= 50;
    res += (seq_t)(n / 25);
    n  %= 25;
    res += (seq_t)(n / 10);
    n  %= 10;
    res += (seq_t)(n / 5);
    n  %= 5;
    res += (seq_t)n;            /* remaining pennies */
    return res;
}
