/*
 * A133806  Alternate terms of A131708 and A000079.
 * https://oeis.org/A133806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the linear recurrence a(n) = 2*a(n-6) + 3*a(n-2) -
 *            3*a(n-4) for n ≥ 6, with initial values a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=2, a(4)=2, a(5)=4
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula is derived from the generating function of A133806
   and provides all terms, including the alternating powers of two
   on odd indices. */

seq_t a(idx_t n)
{
    /* Base terms for n = 0..5 */
    static const seq_t base[6] = {0, 1, 1, 2, 2, 4};

    /* Trivial cases */
    if (n < 6)
        return base[n];

    /* Rolling buffer of the last six values:
       buf[0] = a(n-6), buf[1] = a(n-5), ..., buf[5] = a(n-1) */
    seq_t buf[6];
    buf[0] = base[0];
    buf[1] = base[1];
    buf[2] = base[2];
    buf[3] = base[3];
    buf[4] = base[4];
    buf[5] = base[5];

    /* Iterate from 6 up to n computing a(cur) */
    for (idx_t cur = 6; cur <= n; ++cur) {
        /* Compute a(cur) using the recurrence:
           a(cur) = 2*a(cur-6) + 3*a(cur-2) - 3*a(cur-4)  */
        seq_t val = 2 * buf[0] + 3 * buf[4] - 3 * buf[2];

        /* Shift buffer left: discard the oldest value and
           insert the newly computed term at the end. */
        for (int k = 0; k < 5; ++k)
            buf[k] = buf[k + 1];
        buf[5] = val;
    }

    /* After the loop, buf[5] holds a(n) */
    return buf[5];
}
