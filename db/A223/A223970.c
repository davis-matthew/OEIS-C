/*
 * A223970  Number of n X 3 0..2 arrays with rows and antidiagonals unimodal.
 * https://oeis.org/A223970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 43*a(n-2) - 125*a(n-3) + 283*a(n-4) -
 *            70*a(n-5) - 111*a(n-6) + 45*a(n-7) Initial values: a(1)=22,
 *            a(2)=484, a(3)=9515, a(4)=186004, a(5)=3628696, a(6)=70779056,
 *            a(7)=1380511272 (OEIS offset = 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) … a(7) */
    static const seq_t init[7] = {
        22,
        484,
        9515,
        186004,
        3628696,
        70779056,
        1380511272
    };

    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, but safe */
    if (n <= 7)
        return init[n-1];

    /* buffer holding last seven terms: buf[0]=a(i-7), … , buf[6]=a(i-1)  */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    /* iterate from 8 up to n */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t next =
            (seq_t)22 * buf[6]          /* a(i-1)   */
          - (seq_t)43 * buf[5]          /* a(i-2)   */
          - (seq_t)125 * buf[4]         /* a(i-3)   */
          + (seq_t)283 * buf[3]         /* a(i-4)   */
          - (seq_t)70  * buf[2]         /* a(i-5)   */
          - (seq_t)111 * buf[1]         /* a(i-6)   */
          + (seq_t)45  * buf[0];        /* a(i-7)   */

        /* shift buffer left, insert new term at position 6 */
        for (idx_t j = 0; j < 6; ++j)
            buf[j] = buf[j+1];
        buf[6] = next;
    }

    return buf[6];   /* a(n) is now at the last position */
}
