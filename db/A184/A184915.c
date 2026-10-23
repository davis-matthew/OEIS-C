/*
 * A184915  n+[rn/u]+[sn/u]+[tn/u], where []=floor and r=2^(1/5), s=r^2,
 *          t=r^3, u=r^4.
 * https://oeis.org/A184915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 72, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A184915
 *
 *  Formula:
 *      a(n) = n + ⌊n / 2^(3/5)⌋ + ⌊n / 2^(2/5)⌋ + ⌊n / 2^(1/5)⌋
 *  The fractions are obtained from r = 2^(1/5), s = r^2, t = r^3,
 *  u = r^4, so that r/u = 2^(-3/5), s/u = 2^(-2/5), t/u = 2^(-1/5).
 *
 *  The floors are computed using only integer arithmetic by
 *  exploiting the identity
 *      ⌊n / 2^(k/5)⌋ = max{ m | m^5 * 2^k ≤ n^5 }    (k = 1,2,3).
 *  A small binary search on m (0 ≤ m ≤ n) gives the desired value.
 */

#include <stdint.h>   /* for uint64_t */

static seq_t pow5(seq_t x) {
    seq_t t2 = x * x;        /* x² */
    seq_t t3 = t2 * x;       /* x³ */
    seq_t t4 = t2 * t2;      /* x⁴ */
    return t4 * x;           /* x⁵ */
}

/* floor_n_over_root5_k(n, k, n5)
 *  Computes ⌊n / 2^(k/5)⌋ where k == 1, 2, or 3,
 *  with n5 = n^5.
 */
static seq_t floor_n_over_root5_k(idx_t n, int k, seq_t n5) {
    seq_t low = 0;
    seq_t high = n;                 /* floor value never exceeds n */
    seq_t two_pow = (seq_t)1 << k;  /* 2^k */

    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t mid_pow5 = pow5(mid);

        /* mid_pow5 * 2^k <= n5 ? */
        if (mid_pow5 * two_pow <= n5)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* offset is 1, but return 0 for n=0 */

    seq_t nseq = (seq_t)n;
    /* Compute n^5 once for all three floors */
    seq_t n2 = nseq * nseq;          /* n² */
    seq_t n3 = n2 * nseq;            /* n³ */
    seq_t n4 = n3 * nseq;            /* n⁴ */
    seq_t n5 = n4 * nseq;            /* n⁵ */

    /* Compute the three floor terms */
    seq_t f1 = floor_n_over_root5_k(n, 1, n5);  /* ⌊n / 2^(1/5)⌋ */
    seq_t f2 = floor_n_over_root5_k(n, 2, n5);  /* ⌊n / 2^(2/5)⌋ */
    seq_t f3 = floor_n_over_root5_k(n, 3, n5);  /* ⌊n / 2^(3/5)⌋ */

    return nseq + f1 + f2 + f3;
}
