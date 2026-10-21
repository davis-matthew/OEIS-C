/*
 * A065756  Generalization of the Genocchi numbers given by the Gandhi
 *          polynomials A(n+1,r) = r^5 A(n, r + 1) - (r - 1)^5 A(n, r); A(1,r)
 *          = r^5 - (r-1)^5.
 * https://oeis.org/A065756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A(n-1,1) where A(1,r) = r^5-(r-1)^5 and A(n+1,r)=r^5
 *            A(n,r+1)-(r-1)^5 A(n,r)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Return the n‑th term of OEIS sequence A065756 (1‑based). */
seq_t a(idx_t n)
{
    if (n <= 2)         /* a(1)=a(2)=1 */
        return (seq_t)1;

    /* Allocate array for A(1,r) with r = 1 .. n (index 0 unused). */
    idx_t len = n;
    seq_t *cur = (seq_t *)malloc((len + 1) * sizeof(seq_t));
    if (!cur) return (seq_t)0;   /* failure, should not happen */

    /* Base step: A(1,r) for all r. */
    for (idx_t r = 1; r <= len; ++r) {
        seq_t rs = (seq_t)r;
        seq_t r1s = (seq_t)(r - 1);
        seq_t r5   = rs * rs * rs * rs * rs;
        seq_t r1_5 = r1s * r1s * r1s * r1s * r1s;
        cur[r] = r5 - r1_5;
    }

    /* Iterate n-2 times to reach A(n-1,1). */
    for (idx_t step = 1; step <= n - 2; ++step) {
        idx_t newlen = len - 1;
        seq_t *next = (seq_t *)malloc((newlen + 1) * sizeof(seq_t));
        if (!next) { free(cur); return (seq_t)0; }

        for (idx_t r = 1; r <= newlen; ++r) {
            seq_t rs   = (seq_t)r;
            seq_t r1s  = (seq_t)(r - 1);
            seq_t r5   = rs * rs * rs * rs * rs;
            seq_t r1_5 = r1s * r1s * r1s * r1s * r1s;
            /* A(n+1,r) = r^5 * A(n,r+1) - (r-1)^5 * A(n,r) */
            next[r] = r5 * cur[r + 1] - r1_5 * cur[r];
        }

        free(cur);
        cur = next;
        len = newlen;
    }

    /* Now cur[1] == A(n-1,1) == a(n). */
    seq_t result = cur[1];
    free(cur);
    return result;
}
