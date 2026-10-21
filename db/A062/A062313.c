/*
 * A062313  Factorials of nonprime numbers.
 * https://oeis.org/A062313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (nth nonprime number)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Nonprime numbers: 1 (by convention) and all composite integers.   */
/*  The function computes the factorial of the n‑th nonprime in ascending order.   */

#include <stdbool.h>
#include <stdint.h>

/*  Return true if x is a prime number (x >= 2).  */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    seq_t factorial = (seq_t)1;
    idx_t count = 0;                      /* number of nonprimes seen */
    for (idx_t i = 1; ; ++i) {
        factorial = factorial * (seq_t)i;   /* factorial of i */

        if (!is_prime(i)) {                /* i is nonprime */
            ++count;
            if (count == n)
                return factorial;
        }
    }
}
