/*
 * A194247  [sum{(rk) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=3-sqrt(5).
 * https://oeis.org/A194247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sum_{k=1}^n (3-sqrt(5))*k - sum_{k=1}^n
 *            floor((3-sqrt(5))*k) ) Using the identities floor(k*(3-sqrt(5)))
 *            = 3*k - floor(k*sqrt(5)) - 1 and floor(k*sqrt(5)) = floor(
 *            sqrt(5*k^2) ), we reduce the problem to integer arithmetic only
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Helper: integer square root for unsigned __int128.
   Returns floor(sqrt(n)).  */
static inline seq_t isqrt_u128(unsigned __int128 n)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == n)
            return (seq_t)mid;
        else if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (seq_t)high;   /* high * high <= n < (high+1)^2 */
}

/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t sum_floor_sqrt5 = 0;          /* T(n) = sum_{k=1}^n floor(k*sqrt(5)) */

    /* compute T(n) */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned __int128 val = (unsigned __int128)5 * k * k; /* 5*k^2 */
        seq_t f = isqrt_u128(val);                          /* floor(k*sqrt(5)) */
        sum_floor_sqrt5 += f;
    }

    /* compute L = n*(n+1)/2 and floor(L*sqrt(5)) */
    unsigned __int128 L   = (unsigned __int128)n * (n + 1) / 2;
    unsigned __int128 valL = (unsigned __int128)5 * L * L; /* 5*L^2 */
    seq_t floor_L_sqrt5 = isqrt_u128(valL);                /* floor(L*sqrt(5)) */

    /* a(n) = n - 1 - floor(L*sqrt(5)) + sum_{k=1}^n floor(k*sqrt(5)) */
    seq_t result = (seq_t)(n - 1) - floor_L_sqrt5 + sum_floor_sqrt5;
    return result;
}
