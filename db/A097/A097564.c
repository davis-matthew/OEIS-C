/*
 * A097564  a(n) = (a(n-1) mod 2)*a(n-1) + a(n-2) with a(0)=0, a(1)=1.
 * https://oeis.org/A097564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n) = (a(n-1) mod 2)*a(n-1)+a(n-2)
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 76, u64 n <= 152,
 *            u128 n <= 304, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (uses only integer arithmetic and is O(n) in n) */
seq_t a(idx_t n)
{
    /* handle the trivial base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* a0 = a(i-2), a1 = a(i-1) during the loop */
    seq_t a0 = (seq_t)0;          /* a(0) */
    seq_t a1 = (seq_t)1;          /* a(1) */
    seq_t ai;

    /* iterate from i = 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        /* ai = a(n) depending on the parity of a1 (a(n-1)) */
        if ((a1 & (seq_t)1) == (seq_t)0)        /* a(n-1) is even */
            ai = a0;                          /* a(n) = a(n-2) */
        else
            ai = a1 + a0;                     /* a(n) = a(n-1) + a(n-2) */

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = ai;
    }

    return a1; /* a(n) */
}
