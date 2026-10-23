/*
 * A190507  n+[nr/u]+[ns/u]+[nt/u]; r=golden ratio, s=r+1, t=r+2, u=r+3.
 * https://oeis.org/A190507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*(1+3√5)/22⌋ + ⌊n*(4+√5)/11⌋ + ⌊n*(15+√5)/22⌋
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where √5 is the square root of 5. */

#include <stdint.h>
#include <math.h>

/* high‑precision scaling factor (2^62 fits in 64‑bit) */
static const uint64_t Q = 1ULL << 62;

/* compute floor(√5 * Q) once (≈ 1.026e19) */
static uint64_t get_sqrt5_num(void)
{
    static uint64_t val = 0;
    if (!val) {
        long double sqrt5 = sqrtl((long double)5.0L);
        /* use __int128 to avoid overflow before truncation */
        __int128 tmp = (__int128)(sqrt5 * (long double)Q);
        val = (uint64_t)tmp;
    }
    return val;
}

/* compute floor( n * (A + B√5) / D ) in a robust way */
static uint64_t floor_term(idx_t n, uint64_t A, uint64_t B, uint64_t D)
{
    uint64_t sqrt5_num = get_sqrt5_num();             /* floor(√5 * Q)          */
    __int128 n128 = ( __int128 ) n;                  /* n as 128‑bit          */
    __int128 denom = ( __int128 ) D * Q;              /* D * Q                 */

    /* numerator with lower bound of √5 */
    __int128 num_low = n128 * ( __int128 ) A * Q
                    + n128 * ( __int128 ) B * sqrt5_num;

    uint64_t val = (uint64_t)( num_low / denom );     /* floor with lower bound */

    /* numerator with upper bound of √5 (add one to sqrt5_num) */
    __int128 num_up = n128 * ( __int128 ) A * Q
                    + n128 * ( __int128 ) B * ( sqrt5_num + 1 );

    /* check whether the true value reaches the next integer */
    __int128 next_thresh = ( (__int128)( val + 1 ) * D ) * Q;
    if ( num_up >= next_thresh )
        ++val;

    return val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts from 1, value for n=0 is irrelevant */

    __int128 total = (__int128) n;

    total += floor_term(n, 1, 3, 22);   /* floor(n*(1+3√5)/22)   */
    total += floor_term(n, 4, 1, 11);   /* floor(n*(4+√5)/11)    */
    total += floor_term(n, 15, 1, 22);  /* floor(n*(15+√5)/22)   */

    return (seq_t) total;
}
