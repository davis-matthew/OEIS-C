/*
 * A056641  Least positive integer k for which (b+1)^k is not palindromic in
 *          base b, b = 2, 3, 4, ...
 * https://oeis.org/A056641
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive integer k such that (n+1)^k is NOT
 *            palindromic in base n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 30, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2; for other n we return 0 */
    if (n < 2)
        return (seq_t)0;

    idx_t base = n;
    seq_t base1 = (seq_t)(base + 1);   /* value b+1 */
    seq_t power = base1;               /* (b+1)^1 */
    idx_t k = 1;

    for (;;) {
        if (!is_palindrome(power, base))
            return (seq_t)k;
        power *= base1;   /* compute next power (b+1)^k */
        ++k;
    }
}
