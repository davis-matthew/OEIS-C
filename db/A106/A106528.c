/*
 * A106528  Values of y in x^2 - 289 = 2*y^2.
 * https://oeis.org/A106528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 34, u64 n <= 72, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106528: Values of y in x^2 - 289 = 2*y^2
   Recurrence: a(n) = 6*a(n-3) - a(n-6)   (n >= 7)
   Initial values: a(1)=6, a(2)=20, a(3)=34, a(4)=56, a(5)=126, a(6)=204
   Works for any n >= 1. */
seq_t a(idx_t n)
{
    static const seq_t init[6] = {6, 20, 34, 56, 126, 204};

    if (n <= 6)
        return init[n - 1];

    /* Circular buffer holding the last six sequence values. */
    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i)
        buf[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        /* Indices into the circular buffer */
        idx_t idx   = (i - 1) % 6;   /* position of a(i-1) currently in buf */
        idx_t p3    = (i - 4) % 6;   /* position of a(i-3)   */
        idx_t p6    = (i - 7) % 6;   /* position of a(i-6)   */

        /* Compute next term using the recurrence */
        seq_t next = 6 * buf[p3] - buf[p6];

        /* Store it, overwriting a(i-1) which won't be needed again */
        buf[idx] = next;
    }

    return buf[(n - 1) % 6];
}
