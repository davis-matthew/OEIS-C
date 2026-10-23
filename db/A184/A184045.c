/*
 * A184045  1/9 the number of (n+1) X 7 0..2 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3⋅2^(n/2) + 2^(n+1) + 145 for n even a(n) = 2^(n+1) +
 *            2^((n+3)/2) + 145 for n odd (Same as the recurrence a(n) =
 *            3⋅a(n‑1) – 6⋅a(n‑3) + 4⋅a(n‑4) for n≥5
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"  /* Provides seq_t pow2(idx_t exp); */

seq_t a(idx_t n) {
    /* Offset starts at 1 – values for n<1 are undefined. */
    if (n < 1) return (seq_t)0;

    /* 2^(n+1) term (always present). */
    seq_t res = pow2(n + 1);
    res += (seq_t)145;          /* constant 145 */

    if ((n & 1) == 0) {         /* even n */
        seq_t t = pow2(n / 2);  /* 2^(n/2) */
        res += (seq_t)3 * t;    /* 3⋅2^(n/2) */
    } else {                    /* odd n */
        seq_t t = pow2((n + 3) / 2);  /* 2^((n+3)/2) */
        res += t;                   /* +2^((n+3)/2) */
    }
    return res;
}
