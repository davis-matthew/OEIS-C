/*
 * A121031  Multiples of 11 containing an 11 in their decimal representation.
 * https://oeis.org/A121031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth multiple of 11 that contains the substring "11" in
 *            its decimal representation
 * Range:     u8 n <= 2, u16 n <= 253, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* helper to test if a decimal number contains the consecutive digits 1,1 */
static bool contains_11(seq_t x)
{
    int prev = -1;
    while (x != 0) {
        int digit = (int)(x % 10);
        if (digit == 1 && prev == 1) return true;
        prev = digit;
        x = x / 10;
    }
    return false;
}

/* OEIS sequence a(n) for A121031 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    idx_t k = 1;           /* multiplier index for multiples of 11 */
    for (;;) {
        seq_t val = (seq_t)11 * k;
        if (contains_11(val)) {
            count++;
            if (count == n) return val;
        }
        k++;
    }
}
