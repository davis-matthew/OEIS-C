/*
 * A105291  Triangle read by rows: T(m,n) = binomial(m!,n), m>=0, 0 <= n <=
 *          m!.
 * https://oeis.org/A105291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(m!, k) where m is the unique integer such that
 *            sum_{i=0}^{m-1} (i! + 1) <= n < sum_{i=0}^{m} (i! + 1), and k =
 *            n - sum_{i=0}^{m-1} (i! + 1)
 * Range:     u8 n <= 15, u16 n <= 19, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row m containing index n */
    seq_t fact   = 1;   /* current factorial m!  (starts with 0! = 1) */
    seq_t total  = 0;   /* number of terms preceding the current row */
    idx_t  m     = 0;   /* current row index */

    /* Convert n to seq_t for the comparisons that follow */
    seq_t sn = (seq_t)n;

    while (1) {
        seq_t row_len = fact + 1;          /* length of row m  (m! + 1) */
        if (sn < total + row_len) {
            break;                         /* n lies in this row */
        }
        total += row_len;                  /* advance past this row */
        m += 1;
        fact *= (seq_t)(m);                /* update factorial to (m)! */
    }

    /* Position of the element inside the found row */
    seq_t k = sn - total;                  /* 0 <= k <= fact */

    /* Compute binomial(fact, k) multiplicatively.
       Reduce k using symmetry to minimize loops. */
    if (k > fact - k) {
        k = fact - k;
    }

    seq_t result = 1;
    idx_t k_idx = (idx_t)k;                /* k fits in idx_t for the requested n */

    for (idx_t i = 1; i <= k_idx; ++i) {
        seq_t numerator = fact - k + (seq_t)i;
        result = result * numerator / (seq_t)i;   /* integer division is exact */
    }
    return result;
}
