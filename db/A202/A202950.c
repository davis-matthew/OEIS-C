/*
 * A202950  a(n) = Sum_{k=0..n} (2*n-k)!*2^(k-n)/k!.
 * https://oeis.org/A202950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} (2n‑k)!·2^(k‑n)/k! Recurrence derived in OEIS:
 *            a(0)=1, a(1)=2, a(2)=10 a(n+3) = (12 + 11 n + 2 n²)·a(n+2) + (5
 *            n + 2 n²)·a(n+1) – a(n) + 8
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;

    /* a(n‑3), a(n‑2), a(n‑1) */
    seq_t a_im3 = (seq_t)1;   /* a(0) */
    seq_t a_im2 = (seq_t)2;   /* a(1) */
    seq_t a_im1 = (seq_t)10;  /* a(2) */
    seq_t a_curr = 0;

    for (idx_t i = 3; i <= n; ++i) {
        idx_t k = i - 3; /* current value of n in the recurrence */

        seq_t c1 = (seq_t)12 + (seq_t)11 * k + (seq_t)2 * k * k;     /* 12 + 11n + 2n² */
        seq_t c2 = (seq_t)5 * k + (seq_t)2 * k * k;                 /* 5n + 2n² */

        /* a(i) = c1*a_im1 + c2*a_im2 - a_im3 + 8 */
        a_curr = c1 * a_im1;
        a_curr += c2 * a_im2;
        a_curr += (seq_t)8;
        a_curr -= a_im3;

        /* shift the window for next iteration */
        a_im3 = a_im2;
        a_im2 = a_im1;
        a_im1 = a_curr;
    }

    return a_curr;
}
