/*
 * A168142  Count downwards from 2, then from 8, then from 18, then from ...
 *          2*k^2, k>=1.
 * https://oeis.org/A168142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A168142 is the concatenation of the descending lists 2*k^2,
 *            2*k^2-1, … , 1 for k = 1, 2, 3, …
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the n-th term of OEIS sequence A168142 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* index 0 is undefined but harmless       */

    /* Find the block k such that the cumulative length up to block
       k-1 is smaller than n and the cumulative length up to block
       k is at least n.  C(k) = k(k+1)(2k+1)/3 is the total number of
       terms in the first k blocks.                                                  */
    idx_t k = 1;
    idx_t cum = (k * (k + 1) * (2 * k + 1)) / 3;   /* C(k) */
    while (n > cum) {
        ++k;
        cum = (k * (k + 1) * (2 * k + 1)) / 3;
    }

    /* C(k-1) */
    idx_t prev = 0;
    if (k > 1) {
        idx_t kk = k - 1;
        prev = (kk * (kk + 1) * (2 * kk + 1)) / 3;
    }

    /* Position within the current block (1‑based). */
    idx_t pos = n - prev;

    /* The first term of the block is 2*k^2. */
    seq_t start = (seq_t)2 * (seq_t)k * (seq_t)k;

    /* Within the block we count downwards. */
    return start - (seq_t)(pos - 1);
}
