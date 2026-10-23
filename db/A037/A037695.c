/*
 * A037695  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3,0.
 * https://oeis.org/A037695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the base-4 number formed by the first n digits of the
 *            periodic sequence 1,2,3,0,1,2,3,0,... Recurrence: a(1) = 1; a(n)
 *            = a(n-1)*4 + d_n, where d_n = {1,2,3,0}[ (n-1) mod 4 ]
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against indices below the offset */
    if (n < 1)
        return (seq_t)0;

    /* Periodic digit pattern (1,2,3,0) as seq_t values */
    static const seq_t period[4] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)0 };

    /* Compute the base‑4 number directly; no caching needed. */
    seq_t val = period[0];                /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t d = period[(i - 1) % 4];    /* digit for position i */
        val = val * (seq_t)4 + d;         /* shift left by 4 and add digit */
    }
    return val;
}
