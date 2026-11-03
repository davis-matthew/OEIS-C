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
 * Validated: standard, pass, 116 terms through n = 9543 (2026-11-01)
 * Helpers:   none
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

/* compute smallest and largest prime factors of n (n > 1) */
static inline void factor_min_max(uint64_t n, uint64_t *min_p, uint64_t *max_p)
{
    uint64_t temp = n;
    uint64_t min = 0;
    uint64_t max = 0;

    if ((temp & 1ULL) == 0) {          /* even */
        min = 2;
        max = 2;
        while ((temp & 1ULL) == 0)      /* divide by 2 completely */
            temp >>= 1;
    }

    for (uint64_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            if (min == 0)
                min = i;
            max = i;
            while (temp % i == 0)
                temp /= i;
        }
    }

    if (temp > 1) {                    /* remaining prime factor > sqrt(original n) */
        if (min == 0)
            min = temp;
        max = temp;
    }

    *min_p = min;
    *max_p = max;
}

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
