/*
 * A023747  Plaindromes: numbers whose digits in base 5 are in nondecreasing
 *          order.
 * https://oeis.org/A023747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 329, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Computes the n‑th term of OEIS A023747 (1‑indexed).
   A023747 lists the positive integers whose base‑5 digits are
   non‑decreasing.  The algorithm counts how many such numbers of a
   fixed length exist, determines the length of the n‑th number,
   then chooses the appropriate weakly‑increasing digit sequence
   with combinatorial enumeration.
*/

static idx_t comb_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    /* use the symmetry k = min(k, n-k) to keep the product small */
    if (k > n - k) k = n - k;

    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* just in case the function is called with 0 */
        return 0;

    /* 1. Find the minimal length L such that C(L+4,4) ≥ n. */
    idx_t L = 1;
    while (comb_idx(L + 4, 4) < n)
        ++L;

    /* 2. Count how many numbers of smaller length exist. */
    idx_t prev = 0;
    if (L > 1)
        prev = comb_idx(L + 3, 4);   /* C(L+3,4) */
    /* for L==1 prev stays 0 */

    /* 3. Index within length L (0‑based). */
    idx_t k = n - prev - 1;

    /* 4. Build the k‑th weakly‑increasing digit sequence of length L. */
    seq_t value = 0;
    idx_t prev_digit = 0;          /* the minimal digit allowed for the next position */

    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - pos - 1;                /* digits remaining after this one */
        idx_t lower = (pos == 0 && L > 1) ? 1 : prev_digit;

        for (idx_t d = lower; d <= 4; ++d) {
            /* Count how many sequences start with digit d at this position. */
            idx_t cnt = comb_idx(rem + 4 - d, rem);   /* C(rem + 4 - d, rem) */
            if (k >= cnt) {
                k -= cnt;
            } else {
                value = value * 5 + (seq_t)d;
                prev_digit = d;
                break;
            }
        }
    }

    return value;
}
