/*
 * A116176  a(n) = 9^n * n*(n+1).
 * https://oeis.org/A116176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n * n*(n+1) (offset 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses binary exponentiation for 9^n to keep the algorithm O(log n). */
#include <stdint.h>

static seq_t pow9(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 9;          /* cast to seq_t via implicit conversion */
    while (n) {
        if (n & 1) result *= base;
        base *= base;
        n >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pow = pow9(n);
    seq_t nn  = (seq_t)n;
    seq_t np1 = nn + 1;
    return pow * nn * np1;
}
