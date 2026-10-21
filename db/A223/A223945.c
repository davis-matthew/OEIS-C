/*
 * A223945  Number of n X 4 0..1 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A223945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) - 2*a(n-3) + 7*a(n-4) - 2*a(n-5) -
 *            4*a(n-6) * - 61*a(n-7) + 29*a(n-8) + 8*a(n-9) + 8*a(n-10); *
 *            a(1)=5, a(2)=25, a(3)=96, a(4)=321, a(5)=1062, a(6)=3645,
 *            a(7)=12856, * a(8)=45626, a(9)=161604, a(10)=571202
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial ten terms */
    static const seq_t init[10] = {
        (seq_t)5, (seq_t)25, (seq_t)96, (seq_t)321, (seq_t)1062,
        (seq_t)3645, (seq_t)12856, (seq_t)45626, (seq_t)161604, (seq_t)571202
    };

    if (n == 0) {          /* undefined for n < 1, but return 0 to be safe */
        return (seq_t)0;
    }
    if (n <= 10) {
        return init[n-1];
    }

    /* Use a 10‑slot circular buffer to hold the last 10 values:
     * buffer[0] = a(k-10), buffer[9] = a(k-1) for current k */
    seq_t buf[10];
    for (int i = 0; i < 10; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 11; k <= n; ++k) {
        /* Compute the 5 positive and 5 negative contributions */
        seq_t pos = 5 * buf[9]          /* a(k-1)    */
                + 7 * buf[6]          /* a(k-4)    */
                + 29 * buf[2]         /* a(k-8)    */
                + 8 * buf[1]          /* a(k-9)    */
                + 8 * buf[0];         /* a(k-10)    */

        seq_t neg = 5 * buf[8]          /* a(k-2)    */
                + 2 * buf[7]          /* a(k-3)    */
                + 2 * buf[5]          /* a(k-5)    */
                + 4 * buf[4]          /* a(k-6)    */
                + 61 * buf[3];        /* a(k-7)    */

        seq_t cur = pos - neg;         /* guaranteed non‑negative */

        /* Shift the buffer left by one */
        for (int i = 0; i < 9; ++i) {
            buf[i] = buf[i+1];
        }
        buf[9] = cur;
    }

    return buf[9];
}
