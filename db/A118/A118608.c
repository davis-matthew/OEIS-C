/*
 * A118608  Start with 1 and repeatedly reverse the digits and add 19 to get
 *          the next term.
 * https://oeis.org/A118608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1, a(n)=reverse_decimal(a(n-1))+19 (for n>1)
 * Range:     u8 n <= 18, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined in OEIS, safe guard */
    seq_t val = (seq_t)1;                  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = reverse_decimal(val) + (seq_t)19;
    }
    return val;
}
