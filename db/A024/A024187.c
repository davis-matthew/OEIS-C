/*
 * A024187  n-th elementary symmetric function of 3,4,...,n+3.
 * https://oeis.org/A024187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7 For n ≥ 2: a(n) = (∏_{k=3}^{n+2} k) + (n+3) * a(n-1)
 *            (elementary symmetric sum) This computes the n‑th elementary
 *            symmetric function of the set {3,4,…,n+3}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* below the offset is undefined; return 0 */
    if (n < 1) return (seq_t)0;

    /* a(1)=7 directly */
    if (n == 1) return (seq_t)7;

    seq_t result = (seq_t)7;   /* a(1) */
    seq_t prod   = (seq_t)3;   /* product of numbers 3..(n+2) (for current n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* multiply prod by next element (i+2) to extend product to 3..(i+2) */
        prod *= (seq_t)(i + 2);

        /* update result using recurrence:
           a(i) = prod + (i+3)*a(i-1) */
        result = prod + ((seq_t)(i + 3)) * result;
    }

    return result;
}
