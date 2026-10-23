/*
 * A184918  n+[rn/t]+[sn/t]+[un/t], where []=floor and r=1, s=2^(1/4), t=s^2,
 *          u=s^3.
 * https://oeis.org/A184918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/√2⌋ + ⌊n/2^(1/4)⌋ + ⌊n·2^(1/4)⌋
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only integer arithmetic, no floating point. */

#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) */
static seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo <= hi) {
        seq_t mid = (lo + hi) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Integer fourth root: floor(x^(1/4)) */
static seq_t fourth_root(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = isqrt_seq(x);         /* upper bound: sqrt(x) ≥ fourth root */
    while (lo <= hi) {
        seq_t mid   = (lo + hi) >> 1;
        seq_t sq    = mid * mid;
        seq_t sqsq  = sq * sq;       /* (mid^2)^2 = mid^4 */
        if (sqsq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n<1, but harmless */

    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;               /* n² */

    /* floor(n / √2) */
    seq_t div2   = N2 / 2;           /* floor(n² / 2)               */
    seq_t f_sqrt = isqrt_seq(div2);   /* floor(√(n²/2)) = floor(n/√2) */

    /* n⁴  */
    seq_t N4  = N2 * N2;              /* n⁴ */

    /* floor(n / 2^(1/4))  --  m^4 ≤ n⁴ / 2   */
    seq_t target_over = N4 / 2;        /* floor(n⁴ / 2)               */
    seq_t f_over = fourth_root(target_over);

    /* floor(n * 2^(1/4))  --  m^4 ≤ 2 * n⁴   */
    seq_t target_times = 2 * N4;       /* 2 * n⁴                       */
    seq_t f_times = fourth_root(target_times);

    return N + f_sqrt + f_over + f_times;
}
