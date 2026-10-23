/*
 * A153500  First 3 terms coincide with A152756. For n>3, a(n) is the
 *          palindromic number formed from concatenation of 1, 0,
 *          A147759(n-3), 0, A147759(n-3), 0 and 1.
 * https://oeis.org/A153500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = 101*a(n-1) - 1110*a(n-2) + 102010*a(n-3) - *
 *            111000*a(n-4) + 1010000*a(n-5) - 1000000*a(n-6), n>7 * Initial
 *            values: * a(1)=1, a(2)=101, a(3)=10001, a(4)=1010101, *
 *            a(5)=101101101, a(6)=10101010101, a(7)=1010010100101
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 32‑bit constants that fit in any seq_t type */
    const seq_t ONE   = (seq_t)1;
    const seq_t A2    = (seq_t)101;
    const seq_t A3    = (seq_t)10001;
    const seq_t A4    = (seq_t)1010101;
    const seq_t A5    = (seq_t)101101101;
    const seq_t A6    = (seq_t)10101010101;
    const seq_t A7    = (seq_t)1010010100101;

    if (n == 1) return ONE;
    if (n == 2) return A2;
    if (n == 3) return A3;
    if (n == 4) return A4;
    if (n == 5) return A5;
    if (n == 6) return A6;
    if (n == 7) return A7;

    /* Sliding window of the last six terms */
    seq_t prev[7];
    prev[0] = ONE;
    prev[1] = A2;
    prev[2] = A3;
    prev[3] = A4;
    prev[4] = A5;
    prev[5] = A6;
    prev[6] = A7;

    seq_t cur = 0;
    for (idx_t i = 8; i <= n; ++i) {
        cur = (seq_t)101   * prev[6]
            - (seq_t)1110  * prev[5]
            + (seq_t)102010 * prev[4]
            - (seq_t)111000 * prev[3]
            + (seq_t)1010000 * prev[2]
            - (seq_t)1000000 * prev[1];

        /* shift the window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = prev[6];
        prev[6] = cur;
    }
    return cur;
}
