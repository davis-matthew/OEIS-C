/*
 * A182560  a(n) = (a(n-1) AND a(n-2)) XOR n.
 * https://oeis.org/A182560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A182560: a(0)=0, a(1)=1,
 * a(n) = (a(n-1) AND a(n-2)) XOR n   (n ≥ 2)
 */
seq_t a(idx_t n)
{
    /* Treat the trivial cases directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* a0 = a(i-2), a1 = a(i-1); initialise with a(0) and a(1). */
    seq_t a0 = 0;
    seq_t a1 = 1;
    seq_t an = 0;

    /* Iterate from i = 2 up to and including n. */
    for (idx_t i = 2; i <= n; ++i) {
        an = (a1 & a0) ^ (seq_t)i;  /* (a(n-1) & a(n-2)) ^ n */
        a0  = a1;                   /* shift window: a(i-2) <- a(i-1)   */
        a1  = an;                   /* a(i-1) <- a(i)                   */
    }
    /* After the loop, a1 holds a(n). */
    return a1;
}
