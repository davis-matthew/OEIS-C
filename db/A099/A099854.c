/*
 * A099854  A Chebyshev transform of A048739 related to the knot 8_5.
 * https://oeis.org/A099854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 68, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with initial values: a(0)=1, a(1)=3, a(2)=7, a(3)=14, a(4)=26, a(5)=48.   */
seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    switch (n)
    {
        case 0: return 1;
        case 1: return 3;
        case 2: return 7;
        case 3: return 14;
        case 4: return 26;
        case 5: return 48;
    }

    /* Variables storing a(n-6) .. a(n-1).  Initialize with the first six terms. */
    seq_t p0 = 1;   /* a(n-6)   */
    seq_t p1 = 3;   /* a(n-5)   */
    seq_t p2 = 7;   /* a(n-4)   */
    seq_t p3 = 14;  /* a(n-3)   */
    seq_t p4 = 26;  /* a(n-2)   */
    seq_t p5 = 48;  /* a(n-1)   */
    seq_t new_val;

    idx_t i;
    for (i = 6; i <= n; ++i)
    {
        /* Compute a(i) from the recurrence. */
        new_val = p5 * 3;        /* 3*a(i-1) */
        new_val -= p4 * 4;       /* -4*a(i-2) */
        new_val += p3 * 5;       /* +5*a(i-3) */
        new_val -= p2 * 4;       /* -4*a(i-4) */
        new_val += p1 * 3;       /* +3*a(i-5) */
        new_val -= p0;           /* -a(i-6)  */

        /* Shift the window for the next iteration. */
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = p4;
        p4 = p5;
        p5 = new_val;
    }

    /* After the loop, p5 holds a(n). */
    return p5;
}
