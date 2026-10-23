/*
 * A190770  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(1/2),3,1) and [ ]=floor.
 * https://oeis.org/A190770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3n+1)/sqrt(2)) - 3*floor(n/sqrt(2))
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r = sqrt(1/2) = 1/sqrt(2) */

#include "../utils/floor_sqrt.h"

static inline seq_t floor_div_sqrt2_seq(seq_t x)
{
    /* compute floor(x * r) = floor(x / sqrt(2)) */
    seq_t val = (seq_t)2 * x * x;      /* 2*x^2 */
    seq_t s   = floor_sqrt(val);        /* floor(sqrt(2*x^2)) = floor(x*sqrt(2)) */
    return s / (seq_t)2;                /* floor(x*sqrt(2)/2) = floor(x / sqrt(2)) */
}

seq_t a(idx_t n)
{
    seq_t tn      = (seq_t)3 * (seq_t)n + (seq_t)1;  /* b*n + c */
    seq_t floor_t = floor_div_sqrt2_seq((seq_t)n);
    seq_t floor_tn = floor_div_sqrt2_seq(tn);
    return floor_tn - (seq_t)3 * floor_t;
}
