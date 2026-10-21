/*
 * A238391  Expansion of (1+x)/(1-x^2-3*x^5).
 * https://oeis.org/A238391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 64, u64 n <= 127,
 *            u128 n <= 251, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238391: 
   a(0)=a(1)=a(2)=a(3)=a(4)=1;
   a(n)=3*a(n-5)+a(n-2) for n>4
   Computes a(n) for any n>=0.
   Uses a 6‑element circular buffer so no dynamic allocation is required. */

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)1;

    /* Circular buffer holding the last 6 computed values. */
    seq_t buf[6];
    for (idx_t i = 0; i < 5; ++i)
        buf[i] = (seq_t)1;          /* a(0) .. a(4) */

    seq_t cur = 0;
    for (idx_t i = 5; i <= n; ++i) {
        /* a(i) = 3*a(i-5) + a(i-2) */
        cur = (seq_t)3 * buf[(i - 5) % 6] + buf[(i - 2) % 6];
        buf[i % 6] = cur;          /* store current value for future use */
    }
    return cur;
}
