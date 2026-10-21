/*
 * A098825  Triangle read by rows: T(n,k) = number of partial derangements,
 *          that is, the number of permutations of n distinct, ordered items
 *          in which exactly k of the items are in their natural ordered
 *          positions, for n >= 0, k = n, n-1, ..., 1, 0.
 * https://oeis.org/A098825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r,k) * d(p), where r is the row index such that
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2, p = n − r(r+1)/2, k = r − p, and
 *            d(m) are derangement numbers
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 115, u64 n <= 250,
 *            u128 n <= 661, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r and position p within that row */
    idx_t row = 0;
    idx_t cum = 0;                      /* cumulative terms before current row */
    while (1) {
        idx_t len = row + 1;            /* number of terms in this row */
        if (n < cum + len) {
            break;
        }
        cum += len;
        ++row;
    }
    idx_t p = n - cum;                  /* 0 ≤ p ≤ row  (position in the row) */
    idx_t k = row - p;                  /* original k value */

    /* Compute derangement number d(p) */
    seq_t der_val;
    if (p == 0) {
        der_val = 1;                    /* d(0) = 1 */
    } else if (p == 1) {
        der_val = 0;                    /* d(1) = 0 */
    } else {
        seq_t d0 = 1;                   /* d(0) */
        seq_t d1 = 0;                   /* d(1) */
        seq_t dpi = 0;
        for (idx_t i = 2; i <= p; ++i) {
            dpi = (seq_t)(i - 1) * (d1 + d0);
            d0 = d1;
            d1 = dpi;
        }
        der_val = d1;                   /* d(p) */
    }

    /* Compute binomial coefficient C(row,k) */
    idx_t small_k = (k <= row - k) ? k : row - k;  /* use the smaller side */
    seq_t bin = 1;
    for (idx_t i = 1; i <= small_k; ++i) {
        bin = bin * (row - i + 1) / i;
    }

    return bin * der_val;
}
