/*
 * A133405  a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with initial values 0,0,0,1.
 * https://oeis.org/A133405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4) initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for indices 0‑3, return the initial values directly */
    if (n < 3) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* a(0) … a(3) initialise */
    seq_t v0 = 0;   /* a(n-4) at any step */
    seq_t v1 = 0;   /* a(n-3) */
    seq_t v2 = 0;   /* a(n-2) */
    seq_t v3 = 1;   /* a(n-1) */

    /* iterate from n = 4 up to the desired n,
       using only the last four computed terms */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)3 * v3 - v1 + (seq_t)3 * v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}
