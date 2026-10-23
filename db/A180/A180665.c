/*
 * A180665  Golden Triangle sums: a(n)=a(n-2)+A001654(n) with a(0)=0 and
 *          a(1)=1.
 * https://oeis.org/A180665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + F_n * F_{n+1}; a(0) = 0, a(1) = 1; where F_k is
 *            the k‑th Fibonacci number (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_even = 0;        /* a(n) for even n          */
    seq_t a_odd  = 1;        /* a(n) for odd  n          */

    seq_t F_prev = 0;        /* F_{k-1}  (current F_k will be F_cur) */
    seq_t F_cur  = 1;        /* F_k      */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t F_k   = F_prev + F_cur;   /* F_k     */
        seq_t F_k1  = F_cur + F_k;      /* F_{k+1} */
        seq_t prod  = F_k * F_k1;       /* F_k * F_{k+1} */

        if (k & 1)
            a_odd += prod;   /* a_k for odd k  */
        else
            a_even += prod;  /* a_k for even k */

        /* advance Fibonacci indices for next iteration */
        F_prev = F_cur;
        F_cur  = F_k;
    }

    return (n & 1) ? a_odd : a_even;
}
