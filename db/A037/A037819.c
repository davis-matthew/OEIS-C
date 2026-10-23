/*
 * A037819  Number of i such that d(i) > d(i-1), where Sum_{i=0..m} d(i)*4^i
 *          is the base-4 representation of n.
 * https://oeis.org/A037819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of i≥1 such that the i‑th base‑4 digit of n is
 *            greater than the (i‑1)-th base‑4 digit (with d₀ the least
 *            significant digit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t prev_digit = 0;
    bool first = true;

    while (n > 0) {
        seq_t digit = n & 3;           /* n % 4 */
        if (!first && digit > prev_digit) {
            count++;
        }
        prev_digit = digit;
        first = false;
        n >>= 2;                       /* n /= 4 */
    }

    return count;
}
