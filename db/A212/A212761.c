/*
 * A212761  Number of (w,x,y,z) with all terms in {0,...,n}, w odd, x and y
 *          even.
 * https://oeis.org/A212761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n^3+7*n^2+7*n+1 + (n^2+n-1)*(-1)^n)/16
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t n_seq   = (seq_t)n;
    seq_t n_plus1 = n_seq + (seq_t)1;
    seq_t n2      = n_seq * n_seq;
    seq_t n3      = n2 * n_seq;

    /* 2*n^3 + 7*n^2 + 7*n + 1 */
    seq_t base = ((seq_t)2 * n3) + ((seq_t)7 * n2) +
                 ((seq_t)7 * n_seq) + (seq_t)1;

    /* n^2 + n - 1 (always non‑negative for n ≥ 1) */
    seq_t term2 = n2 + n_seq - (seq_t)1;

    seq_t sum;
    if ((n & 1) == 0)          /* n even: add term2 */
        sum = base + term2;
    else                        /* n odd : subtract term2 */
        sum = base - term2;

    seq_t prod = n_plus1 * sum;
    return prod >> 4;          /* divide by 16 */
}
