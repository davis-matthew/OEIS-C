/*
 * A211415  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2 + x*y = 1.
 * https://oeis.org/A211415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with all terms in
 *            {-n,…,0,…,n} and w^2 + x*y = 1
 * Range:     u8 n <= 14, u16 n <= 424, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9017 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the solutions for fixed |1 - w^2| = m, with m >= 0.  For m = 0
   the number of solutions of xy = 0 in the interval is 4*n + 1.  For m
   > 0, each pair of positive divisors (a,b) with a*b = m and a,b <= n
   yields two sign choices, hence the result is 2 * (# such divisor pairs). */
static seq_t
count_pairs_abs(seq_t m, idx_t n)
{
    if (m == 0) {
        /* x*y = 0: (x,0) for all 2*n+1 x, plus (x,0) with x=0 counted twice,
           hence 4*n + 1. */
        return (seq_t)4 * n + 1;
    }

    seq_t cnt_positive = 0;
    idx_t ms = (idx_t)m;          /* m fits in 64 bits, conservatively cast. */
    for (idx_t a = 1; a * a <= ms; ++a) {
        if (ms % a == 0) {
            idx_t b = ms / a;
            if (a <= n && b <= n) {
                cnt_positive++;
            }
            if (a != b && b <= n && a <= n) {
                cnt_positive++;
            }
        }
    }
    return 2 * cnt_positive;      /* account for the negative divisors. */
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Contribution from w = 0 : d = 1 - 0^2 = 1. */
    result += count_pairs_abs(1, n);

    /* Contributions from w = 1 … n.  For w > 0, |1 - w^2| = w^2 - 1. */
    for (idx_t w = 1; w <= n; ++w) {
        seq_t wsq = (seq_t)w * w;          /* w^2 fits in 64 bits. */
        seq_t m = (w == 1) ? 0 : wsq - 1; /* |1 - w^2| for w > 0. */
        seq_t cp = count_pairs_abs(m, n);
        result += 2 * cp;                 /* account for w = ±w. */
    }

    return result;
}
