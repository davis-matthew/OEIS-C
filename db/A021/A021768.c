/*
 * A021768  Decimal expansion of 1/764.
 * https://oeis.org/A021768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth decimal digit of 1/764
   (decimal expansion 0.00130890052356020942408376963350785340314136125654450261780104712041884816...) */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;           /* current remainder (0 <= rem < 764) */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;           /* bring next digit */
        seq_t digit = (seq_t)(rem / 764);  /* 0 <= digit <= 9 */
        rem  %= 764;
        if (i == n)              /* return the requested digit */
            return digit;
    }
    return 0; /* unreachable */
}
