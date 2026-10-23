/*
 * A191697  a(n) = r1^n + r2^n + r3^n where r1, r2, r3 are the three roots of
 *          x^3 - 2*x - 2 = 0.
 * https://oeis.org/A191697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A191697: a(n) = r1^n + r2^n + r3^n,
 * where r1, r2, r3 are the roots of x^3 - 2*x - 2 = 0.
 * Recurrence: a(1)=0, a(2)=4, a(3)=6,
 * for n≥4: a(n) = 2*a(n-2) + 2*a(n-3)
 */
seq_t a(idx_t n)
{
    /* handle very small indices */
    if (n == 0) return 0;          /* safety – not used in OEIS offset */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;

    /* iterate using the three‑term recurrence */
    seq_t a1 = (seq_t)0;   /* a(1)   */
    seq_t a2 = (seq_t)4;   /* a(2)   */
    seq_t a3 = (seq_t)6;   /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        /* compute a(i) = 2*a(i-2) + 2*a(i-3) */
        seq_t next = (a1 << 1) + (a2 << 1);  /* a1 = a(i-3), a2 = a(i-2) */

        /* shift the window forward */
        a1 = a2;   /* becomes a(i-2) for next iteration */
        a2 = a3;   /* becomes a(i-1) for next iteration */
        a3 = next; /* a(i) */
    }
    return a3;
}
