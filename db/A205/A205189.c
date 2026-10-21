/*
 * A205189  Number of (n+1) X 5 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock differing from each horizontal
 *          or vertical neighbor.
 * https://oeis.org/A205189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4) for n > 6 Initial values:
 *            a(1)=72, a(2)=104, a(3)=160, a(4)=256, a(5)=384, a(6)=576 (The
 *            sequence is defined for n ≥ 1; for n < 1 the function returns
 *            0.)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 44, u64 n <= 97, u128 n <= 204,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    /* return early if the requested term is one of the initial values */
    if (n <= 6) {
        static const seq_t init[7] = {
            0ULL,            /* a(0) unused */
            72ULL, 104ULL, 160ULL,
            256ULL, 384ULL, 576ULL
        };
        return init[n];
    }

    /* For n > 6 compute iteratively using the recurrence.
       We keep the four most recent values:
           v0 = a(i-4), v1 = a(i-3), v2 = a(i-2), v3 = a(i-1)   */
    seq_t v0 = 160;  /* a(3) */
    seq_t v1 = 256;  /* a(4) */
    seq_t v2 = 384;  /* a(5) */
    seq_t v3 = 576;  /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = v3 + v2 - v1 + v0;  /* a(i) = a(i-1)+a(i-2)-a(i-3)+a(i-4) */
        /* shift the window forward */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3; /* v3 holds a(n) after the loop */
}
