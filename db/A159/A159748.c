/*
 * A159748  If an array is made of columns of -nacci sequences, fibo-, tribo-
 *          etc. all starting w. 1,1,2 etc, the NW to SE diagonals can be
 *          extended by computation. The above is diagonal 11. See A159741 for
 *          details.
 * https://oeis.org/A159748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) for n > 8 Initial values: a(1)=144,
 *            a(2)=927, a(3)=2872, a(4)=6930, a(5)=15109, a(6)=31489,
 *            a(7)=64256, a(8)=129792
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 23, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against out of range indices (offset=1) */
    if (n < 1u) return 0;

    /* The first eight terms */
    static const seq_t init[8] = {
        (seq_t)144,   (seq_t)927,
        (seq_t)2872,  (seq_t)6930,
        (seq_t)15109, (seq_t)31489,
        (seq_t)64256, (seq_t)129792
    };

    /* Return a pre‑computed value if within the known range. */
    if (n <= 8u) return init[n - 1];

    /* Iterate the recurrence from the eighth term up to n. */
    seq_t prev2 = init[6];   /* a(7) */
    seq_t prev1 = init[7];   /* a(8) */
    seq_t next;

    for (idx_t i = 9u; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 2*a(i-2) */
        next = (seq_t)3 * prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
