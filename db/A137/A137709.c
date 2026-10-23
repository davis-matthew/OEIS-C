/*
 * A137709  Secondary upper Wythoff sequence.
 * https://oeis.org/A137709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = 2*A001950(n) – 1 * a(2n) = 2*A001950(n) * where
 *            A001950(n) = floor(n*φ²) = floor((n + sqrt(5*n²)) / 2) + n * *
 *            The function computes the n‑th term of OEIS A137709 without
 *            using floating * point arithmetic. It relies on an integer
 *            square‑root routine that works * for any unsigned integra...
 * Range:     u8 n <= 14, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

/* integer square root: floor(sqrt(x)) */
static seq_t floor_sqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= x / mid) {          /* mid*mid <= x  without overflow */
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* upper Wythoff sequence A001950(n) = floor(n*φ²) */
static seq_t upper_wythoff(idx_t n)
{
    /* n must be cast to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;
    seq_t sq = (seq_t)5 * n_seq * n_seq;    /* 5 * n^2  */
    seq_t sqrt_s = floor_sqrt_seq(sq);
    seq_t lower = (n_seq + sqrt_s) / 2;     /* floor(n*φ)   */
    return lower + n_seq;                   /* upper = lower + n */
}

/* OEIS A137709: Secondary upper Wythoff sequence */
seq_t a(idx_t n)
{
    /*  n >= 1 by definition */
    idx_t k;
    seq_t w;

    if (n & 1U) {                /* n is odd: a(2n-1) = 2*w - 1  */
        k = (n + 1) >> 1;
        w = upper_wythoff(k);
        return w * 2 - 1;
    } else {                      /* n is even:  a(2n)   = 2*w   */
        k = n >> 1;
        w = upper_wythoff(k);
        return w * 2;
    }
}
