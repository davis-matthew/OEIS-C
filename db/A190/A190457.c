/*
 * A190457  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(golden ratio,4,3) and
 *          []=floor.
 * https://oeis.org/A190457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4*n+3)*φ) - 4*floor(n*φ) - floor(3*φ) where φ =
 *            (1+√5)/2, floor(3*φ) = 4
 * Range:     u8 none, u16 n <= 13, u32 n <= 132, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper computes floor(k*φ) using integer arithmetic only:
   floor(k*φ) = (k + floor(√(5*k*k)))/2
   The function floor_sqrt is provided by the utilities. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static inline seq_t floor_phi_seq(idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t five_kk = (seq_t)5 * kk * kk;          /* 5*k*k  */
    seq_t sqrt5 = floor_sqrt(five_kk);          /* floor(k*√5) */
    return (kk + sqrt5) / 2;                    /* floor(k*φ) */
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence offset. */
    seq_t fn = floor_phi_seq(n);                 /* floor(n*φ) */
    seq_t m  = (seq_t)4 * (seq_t)n + (seq_t)3;    /* 4n+3  */
    seq_t fm = floor_phi_seq((idx_t)m);          /* floor((4n+3)*φ) */

    return fm - (seq_t)4 * fn - (seq_t)4;         /* floor((4n+3)φ)-4*floor(nφ)-4 */
}
