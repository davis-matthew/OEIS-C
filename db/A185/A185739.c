/*
 * A185739  Accumulation array of A185738, by antidiagonals.
 * https://oeis.org/A185739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c)/2 where T(r,c) = c*(4*(2^r-1)+(c-3)*r) The
 *            sequence is obtained by enumerating the two‑dimensional array
 *            T(r,c) by antidiagonals starting with (r,c) = (1,1)
 * Range:     u8 n <= 24, u16 n <= 131, u32 n <= 523, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Returns the a(n) term of A185739.  idx is 1‑based. */
seq_t a(idx_t idx)
{
    if (idx == 0) return 0;          /* undefined, but safe */

    /* Find the diagonal number:
       t*(t+1)/2 >= idx, where t = d-1 and d is the diagonal index. */
    long double sq = sqrtl((long double)8.0L * (long double)idx + 1.0L);
    idx_t t = (idx_t)((sq - 1.0L) / 2.0L);
    if (t * (t + 1) / 2 < idx) t++;   /* correct possible rounding error */

    /* Index within the diagonal (1‑based) */
    idx_t count_before = (t - 1) * t / 2;      /* elements before this diagonal */
    idx_t p = idx - count_before;              /* position in current diagonal */

    idx_t r = p;                     /* row index in the array (>=1) */
    idx_t c = t + 1 - p;             /* column index (>=1) */

    /* pow2 = 2^r */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < r; ++i) pow2 *= (seq_t)2;

    /* Compute T(r,c)/2 using integer arithmetic without underflow. */
    seq_t term = (seq_t)4 * (pow2 - (seq_t)1);  /* 4*(2^r-1) */

    /* (c-3)*r may be negative for c=1,2; handle sign explicitly. */
    int64_t sub = (int64_t)c - 3;
    sub *= (int64_t)r;                /* small, fits in int64_t */

    if (sub >= 0)
        term += (seq_t)sub;
    else
        term -= (seq_t)(-sub);

    seq_t T = (seq_t)c * term;        /* T(r,c) */
    seq_t result = T / (seq_t)2;      /* final value a(n) */

    return result;
}
