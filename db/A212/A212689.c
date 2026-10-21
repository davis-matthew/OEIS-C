/*
 * A212689  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          2|w-x|>n+|y-z|.
 * https://oeis.org/A212689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4) + a(n-5) -
 *            3*a(n-6) + a(n-7) * for n >= 8, with initial values: * a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=6, a(4)=20, a(5)=58, a(6)=124, a(7)=244
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init_vals[8] = {
    (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)6, (seq_t)20, (seq_t)58,
    (seq_t)124, (seq_t)244
};

seq_t a(idx_t n)
{
    /* Return the known small values directly. */
    if (n < 8) return init_vals[n];

    /*
     * Keep the last eight values in a circular buffer.
     * The buffer is small (8) so a simple shift is fine
     * and works with any seq_t type (including non‑trivial).
     */
    seq_t buf[8];
    for (int k = 0; k < 8; ++k) buf[k] = init_vals[k];

    for (idx_t i = 8; i <= n; ++i)
    {
        seq_t val = 3 * buf[7];
        val -= buf[6];
        val -= 5 * buf[5];
        val += 5 * buf[4];
        val += buf[3];
        val -= 3 * buf[2];
        val += buf[1];

        /* shift the buffer left by one position */
        for (int k = 0; k < 7; ++k) buf[k] = buf[k + 1];
        buf[7] = val;
    }

    return buf[7];
}
