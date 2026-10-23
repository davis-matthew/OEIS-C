/*
 * A180410  Unique digits used in n in numerical order.
 * https://oeis.org/A180410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, look
 * Formula:   a(n) = digits of n sorted ascending, taken as a decimal integer
 *            (leading zeros dropped)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute a(n) – the unique digits of n in increasing order, as an integer.
   For example, a(93077) = 379 (digits 0,3,7,9 sorted → 0379 → 379). */
seq_t a(idx_t n)
{
    unsigned char present[10] = {0};

    if (n == 0)          /* n ≥ 1 in the problem, but handle 0 safely */
        return 0;

    idx_t x = n;
    while (x > 0) {
        unsigned d = x % 10;   /* digit 0–9 */
        present[d] = 1;
        x /= 10;
    }

    seq_t res = 0;
    for (int d = 0; d < 10; ++d) {
        if (present[d])
            res = res * 10 + d;   /* build decimal number, dropping leading 0s */
    }

    return res;
}
