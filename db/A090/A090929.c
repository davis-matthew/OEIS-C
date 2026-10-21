/*
 * A090929  Permutation of natural numbers arising from a square spiral.
 * https://oeis.org/A090929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = value at (x,y) = n in the counter‑clockwise * square
 *            spiral, rotated 90° clockwise about the origin * to obtain
 *            (x',y'), then the number at (x',y') is a(n)
 * Range:     u8 n <= 225, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"  /* provides _floor_isqrt_u64 */

seq_t a(idx_t n)
{
    /* n is 1‑based.  n == 1 maps to itself. */
    if (n <= 1U)
        return (seq_t)n;

    /* Compute the ring r containing n.  The maximum number in ring r is
     * s² where s = 2r + 1 is an odd integer.  Find the smallest odd s
     * with s² ≥ n.  s is obtained by taking the floor square root of n
     * and adjusting. */
    uint64_t n_val = n;
    uint64_t s = _floor_isqrt_u64(n_val);      /* floor √n   */
    if (s * s < n_val)                         /* make it ceil √n */
        ++s;
    if ((s & 1ULL) == 0)                       /* ensure s is odd */
        ++s;
    uint64_t r = (s - 1U) >> 1;                /* r = (s‑1)/2 */

    /* Side length of the ring (length of one edge, 2r). */
    seq_t side_len = (seq_t)r * 2U;

    /* Minimum number in this ring: (2r‑1)² + 1.  s_seq = 2r+1. */
    seq_t s_seq = (seq_t)r * 2U + 1U;
    seq_t min_n_seq = ((s_seq - 2U) * (s_seq - 2U)) + 1U;

    /* Offset of n within the ring. */
    seq_t n_seq = (seq_t)n_val;
    seq_t t = n_seq - min_n_seq;                 /* 0 ≤ t < 8r */

    /* Determine which side and the offset along that side. */
    seq_t side   = t / side_len;                 /* 0 … 3 */
    seq_t offset = t % side_len;                 /* 0 … 2r‑1 */

    /* Convert to 64‑bit signed coordinates.  r fits in int64_t. */
    int64_t rr = (int64_t)r;
    int64_t off = (int64_t)offset;
    int64_t x, y;

    switch ((unsigned)side) {
        case 0:   /* right side: from ( r , -r+1 ) to ( r ,  r ) */
            x =  rr;
            y = -rr + 1 + off;
            break;
        case 1:   /* top side:   from ( r‑1 ,  r ) to ( -r ,  r ) */
            x =  rr - 1 - off;
            y =  rr;
            break;
        case 2:   /* left side:  from ( -r ,  r‑1 ) to ( -r , -r ) */
            x = -rr;
            y =  rr - 1 - off;
            break;
        default:  /* bottom side: from ( -r+1 , -r ) to ( r , -r ) */
            x = -rr + 1 + off;
            y = -rr;
            break;
    }

    /* Rotate 90° clockwise: (x , y) → (y , -x). */
    int64_t xr = y;
    int64_t yr = -x;

    /* Now find the index of the rotated coordinate (xr, yr). */
    if (xr == 0 && yr == 0)
        return (seq_t)1U;                 /* center point */

    /* Identify the ring r2 containing (xr, yr). */
    uint64_t ux = (xr >= 0) ? (uint64_t)xr : (uint64_t)(-xr);
    uint64_t uy = (yr >= 0) ? (uint64_t)yr : (uint64_t)(-yr);
    uint64_t r2 = (ux > uy) ? ux : uy;    /* r2 = max(|xr|,|yr|) */

    seq_t r2_seq = (seq_t)r2;
    seq_t min_n_seq2 = ((r2_seq * 2U - 1U) * (r2_seq * 2U - 1U)) + 1U;

    /* Compute the offset t2 for (xr, yr). */
    seq_t t2;
    if (xr == (int64_t)r2 && yr >= - (int64_t)r2 + 1 && yr <= (int64_t)r2) {     /* right side */
        t2 = (seq_t)(yr - ((int64_t)(-r2 + 1)));
    } else if (yr == (int64_t)r2 && xr >= - (int64_t)r2 && xr <= (int64_t)r2 - 1) { /* top side */
        t2 = r2_seq * 2U + (seq_t)(r2 - 1 - xr);
    } else if (xr == -(int64_t)r2 && yr >= - (int64_t)r2 && yr <= (int64_t)r2 - 1) { /* left side */
        t2 = r2_seq * 4U + (seq_t)(r2 - 1 - yr);
    } else { /* bottom side: yr == -r2 */
        t2 = r2_seq * 6U + (seq_t)(xr + r2 - 1);
    }

    return min_n_seq2 + t2;
}
