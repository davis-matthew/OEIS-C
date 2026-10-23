/*
 * A135670  Triangular sequence of the coefficients of the denominator of the
 *          rational recursive sequence for tan(n*x).
 * https://oeis.org/A135670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 53, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A135670:  a(n) is the coefficient at linear index n in the flattened
   triangular array of the denominator polynomial of tan(r·x) written
   in ascending powers of t = tan(x).  The coefficient of every odd
   power is zero.  Let r be the row index (0‑based) determined from n by
   counting the lengths of the rows where each even indexed row has
   length r+1 and each odd indexed row has length r.  Then for a local
   table index p = 0 … L(r)-1 (where L(r) = r+1 if r even, r if r odd):

        if p is odd          →  a(n) = 0
        else
             k      = p/2
             m      = r/2          // floor
             sign   = (m%2==0) ? +1 : -1   // sign adjustment to make leading
                                           // coefficient positive
             if r is even:   coeff = (-1)^k * C(r, 2k)
             if r is odd:    coeff = (-1)^k * C(r, r-2k)

   Finally a(n) = sign · coeff. */

static inline seq_t binom_val(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;            /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r and the local position pos within that row. */
    idx_t cumulative = (idx_t)0;
    idx_t r = (idx_t)0;
    idx_t pos = (idx_t)0;
    while (1) {
        idx_t L = (r & (idx_t)1) ? r : r + (idx_t)1; /* row length */
        if (n < cumulative + L) {
            pos = n - cumulative;
            break;
        }
        cumulative += L;
        ++r;
    }

    /* Odd positions are zero. */
    if (pos & (idx_t)1) return (seq_t)0;

    /* Even position: compute coefficient. */
    idx_t k = pos / (idx_t)2;
    idx_t m = r / (idx_t)2;
    seq_t sign_factor = ((m & (idx_t)1) == (idx_t)0) ? (seq_t)1 : (seq_t)-1;

    seq_t bin;
    if (r & (idx_t)1) {                      /* odd r */
        idx_t sub = r - (idx_t)2 * k;        /* r - 2k */
        bin = binom_val(r, sub);
    } else {                                 /* even r */
        bin = binom_val(r, (idx_t)2 * k);
    }

    seq_t raw = (k & (idx_t)1) ? -bin : bin;
    return sign_factor * raw;
}
