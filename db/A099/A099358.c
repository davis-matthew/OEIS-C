/*
 * A099358  a(n) = sum of digits of k^4 as k runs from 1 to n.
 * https://oeis.org/A099358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum_{k=1}^{n} digit_sum(k^4)
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* local helper: sum the decimal digits of an unsigned 64‑bit integer */
static seq_t digit_sum_uint64(uint64_t x)
{
    seq_t s = 0;
    while (x)
    {
        s += (seq_t)(x % 10);
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* safety – not part of OEIS offset */
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        uint64_t kk = (uint64_t)k;
        uint64_t k4 = kk * kk * kk * kk;    /* k^4 */
        sum += digit_sum_uint64(k4);
    }
    return sum;
}
