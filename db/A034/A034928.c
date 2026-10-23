/*
 * A034928  Triangle of ballot numbers.
 * https://oeis.org/A034928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   A034928 – triangle of ballot numbers. Triangle definition:
 *            a(0,0)=a(0,1)=1, a(n,0)=a(n,1)=1, a(n,2)=n,
 *            a(n,k)=a(n-1,k)+a(n,k-1)+a(n-1,k-2)-a(n-1,k-1) (2≤k≤n),
 *            a(n,n+1)=a(n,n). The function a(idx_t n) returns the n‑th
 *            element of this triangle read row‑major (a(0,0), a(0,1), a(1,0),
 *            a(1,1), a(1,2),...
 * Range:     u8 n <= 51, u16 n <= 131, u32 n <= 372, u64 n <= 1170,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n);
seq_t a(idx_t n)
{
    /* Find the row that contains index n. */
    idx_t row = 0;
    while (1) {
        /* cumulative terms up to row r:
           (r+1)*(r+4)/2.  Use idx_t arithmetic to avoid overflow. */
        idx_t val = (row + 1) * (row + 4) / 2;
        if (val > n)
            break;
        ++row;
    }

    /* column index of the element in that row. */
    idx_t prev_count = row * (row + 3) / 2;
    idx_t col = n - prev_count;

    /* For row 0 all elements are 1. */
    if (row == 0)
        return (seq_t)1;

    /* Static buffers to hold the previous and current row. */
    static seq_t prev[200] = {0};
    static seq_t curr[200] = {0};

    /* Initialise row 0. */
    prev[0] = prev[1] = (seq_t)1;

    /* Build successive rows up to the required row. */
    for (idx_t i = 1; i <= row; ++i) {
        /* a(i,0)=a(i,1)=1. */
        curr[0] = curr[1] = (seq_t)1;
        /* a(i,2)=i  (valid for all i≥1). */
        curr[2] = (seq_t)i;

        /* Compute k = 3 … i+1. */
        for (idx_t k = 3; k <= i + 1; ++k) {
            seq_t term = 0;

            if (k <= i)               /* a(i-1,k)   */
                term += prev[k];
            term += curr[k - 1];       /* a(i,k-1)   */
            if (k - 2 <= i)            /* a(i-1,k-2) */
                term += prev[k - 2];
            if (k - 1 <= i)            /* a(i-1,k-1) */
                term -= prev[k - 1];

            curr[k] = term;
        }

        /* Prepare for next iteration. */
        for (idx_t k = 0; k <= i + 1; ++k)
            prev[k] = curr[k];
    }

    return curr[col];
}
