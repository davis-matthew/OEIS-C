/*
 * A152152  a(n) = Product_{k=1..n} (1 + 4*sin(2*Pi*k/n)^2).
 * https://oeis.org/A152152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + (-1)^n - Lucas(n))^2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas(n) is defined by L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* a(0) = 0 by definition */

    /* Compute Lucas(n) iteratively */
    seq_t lucas;
    if (n == 1)
        lucas = (seq_t)1;          /* L(1) = 1 */
    else
    {
        seq_t prev2 = (seq_t)2;    /* L(0) */
        seq_t prev1 = (seq_t)1;    /* L(1) */
        seq_t curr;
        for (idx_t i = 2; i <= n; ++i)
        {
            curr = prev2 + prev1;  /* L(i) = L(i-2) + L(i-1) */
            prev2 = prev1;
            prev1 = curr;
        }
        lucas = curr;              /* L(n) */
    }

    /* 1 + (-1)^n is 2 for even n, 0 for odd n */
    seq_t base = (n & 1) ? (seq_t)0 : (seq_t)2;

    /* diff = Lucas(n) - base  (lucas >= base for all n >= 1) */
    seq_t diff = lucas - base;

    return diff * diff;            /* square the difference */
}
