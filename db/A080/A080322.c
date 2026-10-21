/*
 * A080322  Determinant of the n X n tridiagonal matrix M with the elements on
 *          the diagonals equal to 1, except M(n,n-1)=M(n-1,n)=n.
 * https://oeis.org/A080322
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-2) + 7*a(n-3) - 6*a(n-4) + 3*a(n-5) -
 *            a(n-6) Initial values (for n = 2 … 7): a(2) = -3 a(3) = -9 a(4)
 *            = -1 a(5) = 24 a(6) = 36 a(7) = 1
 * Range:     i8 n <= 11, i16 n <= 181, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n <= 1)
        return (seq_t)0;          /* value undefined for n < 2 – return 0 */

    /* For n = 2 … 7 return the known values directly */
    static const seq_t init[6] = {
        (seq_t)-3,  /* a(2) */
        (seq_t)-9,  /* a(3) */
        (seq_t)-1,  /* a(4) */
        (seq_t)24,  /* a(5) */
        (seq_t)36,  /* a(6) */
        (seq_t)1    /* a(7) */
    };
    if (n <= 7)
        return init[n - 2];

    /* Compute a(n) for n >= 8 using the 6‑term recurrence.
       Use a circular buffer holding a(n-6)…a(n-1). */
    seq_t buf[6];
    buf[0] = init[0];  /* a(2)   = a(curr-6) when curr = 8 */
    buf[1] = init[1];  /* a(3)   = a(curr-5) */
    buf[2] = init[2];  /* a(4)   = a(curr-4) */
    buf[3] = init[3];  /* a(5)   = a(curr-3) */
    buf[4] = init[4];  /* a(6)   = a(curr-2) */
    buf[5] = init[5];  /* a(7)   = a(curr-1) */

    for (idx_t k = 8; k <= n; ++k) {
        seq_t next = 3 * buf[5]
                   - 6 * buf[4]
                   + 7 * buf[3]
                   - 6 * buf[2]
                   + 3 * buf[1]
                   - buf[0];

        /* shift the buffer: a(curr-6) becomes a(curr-5), … */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;   /* now buf[5] holds a(n) */
    }

    return buf[5];
}
