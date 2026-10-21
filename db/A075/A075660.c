/*
 * A075660  Let f(n) = abs(lpd(n)-gpf(n)), where lpd(n) is the largest proper
 *          divisor of n and gpf(n) is the greatest prime factor of n.
 *          Sequence gives number of iterations for f(n) to reach zero.
 * https://oeis.org/A075660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_min_max
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = number of iterations needed for the function 
   f(x) = |lpd(x) – gpf(x)| to reach zero, where lpd(x) is the largest 
   proper divisor of x and gpf(x) is the greatest prime factor of x. 
   (Iteration count starts at 0 for n=0, and n=1 requires one step -> 0.)
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_min_max.h"

/* f(n) = |lpd(n) – gpf(n)| for n > 1 */
static inline uint64_t f_func(uint64_t n)
{
    uint64_t min_p, max_p;
    factor_min_max(n, &min_p, &max_p);
    uint64_t lpd = n / min_p;          /* largest proper divisor */
    uint64_t gpf = max_p;              /* greatest prime factor */
    return (lpd >= gpf) ? (lpd - gpf) : (gpf - lpd);
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined input, return 0 */

    uint64_t x = (uint64_t)n;
    uint64_t steps = 0;

    while (x != 0) {
        if (x == 1) {                  /* next value is 0 */
            ++steps;
            break;
        }
        x = f_func(x);
        ++steps;
    }
    return (seq_t)steps;
}
