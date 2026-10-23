/*
 * A124793  Numbers in a perpendicular plane intersecting a 3D clockwise
 *          spiral produced by powers of 2.
 * https://oeis.org/A124793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = -a(n-1) + 32*a(n-2) + 32*a(n-3) (n ≥ 4) Initial values:
 *            a(1)=1, a(2)=5, a(3)=35 Offset: 1 (a(0) is irrelevant for calls
 *            with n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle nonsensical arguments – behaviour below the offset does not matter. */
    if (n == 0)
        return 0;

    /* Return the initial terms directly. */
    if (n == 1) return 1;
    if (n == 2) return 5;
    if (n == 3) return 35;

    /* Sliding window: a1 = a(n-3), a2 = a(n-2), a3 = a(n-1). */
    seq_t a1 = 1;   /* a(1) = a(3‑2) */
    seq_t a2 = 5;   /* a(2) = a(3‑1) */
    seq_t a3 = 35;  /* a(3) = a(3)   */

    /* Compute the sequence up to the requested index. */
    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = -a(i-1) + 32*a(i-2) + 32*a(i-3) */
        seq_t term = (a2 << 5) + (a1 << 5) - a3;
        a1 = a2;      /* shift the window */
        a2 = a3;
        a3 = term;
    }

    return a3;
}
