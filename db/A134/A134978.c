/*
 * A134978  Number of 2 X 2 singular integer matrices with entries from
 *          {2,...,n}.
 * https://oeis.org/A134978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   For each pair (a,b) with 2 ≤ a,b ≤ n let g = gcd(a,b), a1 = a/g,
 *            b1 = b/g. The number of (c,d) with ad = bc and 2 ≤ c,d ≤ n
 *            equals max(0, floor(n / a1) ∧ floor(n / b1) – max(ceil(2 / a1),
 *            ceil(2 / b1)) + 1). Summing this over all ordered pairs (a,b)
 *            gives the desired count of singular 2×2 matrice...
 * Range:     u8 n <= 11, u16 n <= 113, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest common divisor of two non‑negative integers. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* For n < 2 there are no valid entries (empty set). */
    if (n < 2) return 0;

    seq_t total = 0;

    /* Since the formula is symmetric in a and b, only iterate a <= b
       and double the contribution for a != b. */
    for (idx_t a = 2; a <= n; ++a) {
        for (idx_t b = a; b <= n; ++b) {

            /* Compute reduced co‑primed factors. */
            idx_t g = gcd_idx(a, b);
            idx_t a1 = a / g;
            idx_t b1 = b / g;

            /* k_min = max( ceil(2/a1), ceil(2/b1) ) */
            idx_t kmin_a = (2 + a1 - 1) / a1;   /* ceil(2/a1) */
            idx_t kmin_b = (2 + b1 - 1) / b1;   /* ceil(2/b1) */
            idx_t kmin = kmin_a > kmin_b ? kmin_a : kmin_b;

            /* k_max = min( floor(n/a1), floor(n/b1) ) */
            idx_t kmax_a = n / a1;  /* floor(n/a1) */
            idx_t kmax_b = n / b1;  /* floor(n/b1) */
            idx_t kmax = kmax_a < kmax_b ? kmax_a : kmax_b;

            if (kmin <= kmax) {
                seq_t add = (seq_t)(kmax - kmin + 1);
                if (a == b) {
                    total += add;
                } else {
                    total += add * 2;   /* (a,b) and (b,a) */
                }
            }
        }
    }
    return total;
}
