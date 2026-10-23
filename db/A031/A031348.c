/*
 * A031348  2-multiplicative persistence: number of iterations of "multiply
 *          2nd powers of digits" needed to reach 0 or 1.
 * https://oeis.org/A031348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of “multiply 2nd powers of digits”
 *            needed to reach 0 or 1 (2‑multiplicative persistence)
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t next_value(seq_t x)
{
    seq_t prod = 1;
    while (x > 0) {
        seq_t d = x % 10;
        prod *= d * d;
        if (prod == 0) break;          /* early exit if a zero digit appears */
        x /= 10;
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* out‑of‑offset case, not used by OEIS */
        return 0;
    seq_t x = (seq_t)n;
    if (x == 0 || x == 1)      /* already 0 or 1 */
        return 0;
    seq_t count = 1;           /* first iteration */
    x = next_value(x);
    while (x != 0 && x != 1) {
        ++count;
        x = next_value(x);
    }
    return count;
}
