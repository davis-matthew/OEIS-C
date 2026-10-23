/*
 * A105844  Numbers n such that 37*n^2 + 37*n + 1 is a square.
 * https://oeis.org/A105844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1)=0, a(2)=24, a(3)=48, a(4)=3576, a(n)=146*a(n-2)+72-a(n-4)
 *            (n ≥ 5)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)24;
    if (n == 3) return (seq_t)48;
    if (n == 4) return (seq_t)3576;

    /* rolling window for a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t v0 = (seq_t)0;      /* a(n-4) */
    seq_t v1 = (seq_t)24;     /* a(n-3) */
    seq_t v2 = (seq_t)48;     /* a(n-2) */
    seq_t v3 = (seq_t)3576;   /* a(n-1) */

    for (idx_t k = 5; k <= n; ++k) {
        seq_t next = (seq_t)146 * v2 + (seq_t)72 - v0; /* a(k) */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }

    return v3;
}
