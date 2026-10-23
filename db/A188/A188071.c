/*
 * A188071  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(3), k=2, [ ]=floor.
 * https://oeis.org/A188071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3)) - floor(2*sqrt(3)) -
 *            floor((n-2)*sqrt(3))
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here we compute floor(k*sqrt(3)) as floor(sqrt(3*k^2)) using integer arithmetic. */

#include "../utils/sqrt_floor.h"

/* Helper to compute floor(k*sqrt(3)) for k >= 0 using integer sqrt. */
static inline seq_t floor_sqrt3_times(idx_t k)
{
    seq_t prod = (seq_t)k;
    prod *= 3;
    prod *= (seq_t)k;          /* prod = 3 * k^2  */
    return sqrt_floor(prod);   /* floor(sqrt(3*k^2)) = floor(k*sqrt(3)) */
}

seq_t a(idx_t n)
{
    /* Sequence offset is 1; behavior for n < 1 is irrelevant. */
    if (n == 1)   /* a(1) = 0  */
        return 0;

    /* For n >= 2 the values are non‑negative and fit in seq_t. */
    seq_t t1 = floor_sqrt3_times(n);          /* floor(n*sqrt(3)) */
    seq_t t3 = floor_sqrt3_times(n - 2);      /* floor((n-2)*sqrt(3)) */
    return t1 - 3 - t3;                       /* floor(2*sqrt(3)) = 3 */
}
