/*
 * A057056  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and a(n) = j(C(n,3)).
 * https://oeis.org/A057056
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j( C(n,3) ) where C(n,3)=n*(n-1)*(n-2)/6 and j(k) is the
 *            column index of number k in the infinite anti‑diagonal numbering
 *            of the rectangle R(i,j)
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 1883, u64 n <= 9827,
 *            u128 n <= 9827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for seq_t using only integer arithmetic. */
static seq_t _isqrt_seq(seq_t v)
{
    seq_t lo = 0;
    seq_t hi = v;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) >> 1;
        /* mid*mid <= v  ==>  mid <= v/mid  (mid>0) */
        if (mid == 0 || mid <= v / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Defined only for n ≥ 3; otherwise return 0. */
    if (n < 3)
        return 0;

    /* C(n,3) = n*(n-1)*(n-2)/6 */
    seq_t comb = (seq_t)n * ((seq_t)n - 1) * ((seq_t)n - 2) / 6;

    /* Find the anti‑diagonal number t such that
       t*(t+1)/2 ≥ comb > (t-1)*t/2. */
    seq_t d = 1 + 8 * comb;                     /* 1 + 8*comb */
    seq_t sqrt_d = _isqrt_seq(d);               /* floor(sqrt(d)) */
    seq_t t = (sqrt_d - 1) >> 1;                /* floor((sqrt_d-1)/2) */

    /* Adjust if t*(t+1)/2 < comb */
    if (t * (t + 1) / 2 < comb)
        ++t;

    /* Compute offset within diagonal */
    seq_t prev_sum = (t - 1) * t / 2;           /* (t-1)*t/2 */
    seq_t offset = comb - prev_sum;             /* 1‑based offset within diagonal */

    /* row index i and column index j */
    seq_t i = offset;                           /* 1‑based row */
    seq_t j = t - i + 1;                        /* 1‑based column */

    return j;
}
