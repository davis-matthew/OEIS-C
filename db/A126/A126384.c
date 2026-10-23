/*
 * A126384  Number of base 30 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1; for n≥1: a(n)=∑_{k=0}^{29} g(n,k) where g(1,k)=1 and for
 *            m≥1: g(m+1,k)=g(m,k)+ (k>0?g(m,k-1):0)+(k<29?g(m,k+1):0)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    static seq_t cur0[30];
    static seq_t cur1[30];
    seq_t *cur = cur0;
    seq_t *next = cur1;

    /* base step: length 1 strings, all digits are allowed */
    for (idx_t d = 0; d < 30; ++d) cur[d] = (seq_t)1;

    /* iterate for lengths 2 .. n */
    for (idx_t step = 2; step <= n; ++step) {
        for (idx_t d = 0; d < 30; ++d) {
            seq_t sum = cur[d];
            if (d > 0)   sum += cur[d - 1];
            if (d + 1 < 30) sum += cur[d + 1];
            next[d] = sum;
        }
        /* swap current and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* total number of strings of length n */
    seq_t result = (seq_t)0;
    for (idx_t d = 0; d < 30; ++d) result += cur[d];
    return result;
}
