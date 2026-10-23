/*
 * A192311  0-sequence of reduction of (3n-2) by x^2 -> x+1.
 * https://oeis.org/A192311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4) + a(n-5); initial
 *            values: a(1)=1, a(2)=1, a(3)=8, a(4)=18, a(5)=44
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the first five terms directly.  init[0] is unused but keeps the
       indexing simple (a(1) → init[1], etc.).  The array is static to avoid
       repeated initialisation. */
    static const seq_t init[6] = {0, 1, 1, 8, 18, 44};

    if (n < 6)          /* n == 0 is undefined for the sequence, but return 0 */
        return init[n];

    /* Initialise the sliding window with the last five values. */
    seq_t a1 = init[1];   /* a(n-5) */
    seq_t a2 = init[2];   /* a(n-4) */
    seq_t a3 = init[3];   /* a(n-3) */
    seq_t a4 = init[4];   /* a(n-2) */
    seq_t a5 = init[5];   /* a(n-1) */

    /* Iterate from the sixth term up to the requested n. */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 3 * a5 - a4 - 3 * a3 + a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
