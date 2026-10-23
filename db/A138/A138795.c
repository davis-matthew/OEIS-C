/*
 * A138795  a(n) = (A138793(n+1)-A138793(n))/10^n.
 * https://oeis.org/A138795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = rev(n+1) * 10^max(0, n-9)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow10, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t inc = (seq_t)n + 1;            /* n+1 as seq_t */
    seq_t rev = reverse_digits(inc);     /* reverse decimal digits of n+1 */
    idx_t exp = (n > 9) ? (n - 9) : 0;   /* number of trailing zeros */
    seq_t pow = pow10(exp);              /* 10^exp */
    return rev * pow;                    /* final term */
}
