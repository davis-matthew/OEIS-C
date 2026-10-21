/*
 * A205965  a(n) = Fibonacci(n)*A001227(n) for n>=1, where A001227(n) is the
 *          number of odd divisors of n.
 * https://oeis.org/A205965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * A001227(n)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 44, u64 n <= 90, u128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    A001227(n) = number of odd divisors of n        */
/*    Fibonacci(n)  provided by utils/fib.h         */

#include <stdint.h>
#include "../utils/fib.h"

/* Count odd divisors of n.
   Returns the count as seq_t.                     */
static seq_t count_odd_divisors(idx_t n)
{
    /* Odd divisors ignore factor 2. */
    idx_t m = n;
    /* Remove all factors of 2. */
    while ((m & 1) == 0 && m > 0)
        m >>= 1;

    seq_t cnt = 1;   /* start with 1 for the empty product */

    for (idx_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            idx_t exp = 0;
            while (m % i == 0) {
                m /= i;
                exp++;
            }
            cnt *= (seq_t)(exp + 1);
        }
    }

    if (m > 1)          /* remaining prime factor > sqrt(original m) */
        cnt *= (seq_t)2; /* contributes exponent 1 -> (1+1) = 2 */

    return cnt;
}

/* Sequence a(n) = Fibonacci(n) * (number of odd divisors of n).   */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* sequence defined for n >= 1; 0 is safe */

    seq_t f = fib(n);
    seq_t oddcnt = count_odd_divisors(n);
    return f * oddcnt;
}
