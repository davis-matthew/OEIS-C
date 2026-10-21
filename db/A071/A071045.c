/*
 * A071045  Number of 0's in n-th row of triangle in A071030.
 * https://oeis.org/A071045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * a(0)=0, a(1)=0, a(2)=3, a(3)=1; * a(n) = 2*a(n-2) -
 *            a(n-4) for n ≥ 4
 * Range:     u8 n <= 171, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0u || n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)3;
    if (n == 3u) return (seq_t)1;

    /* Store the four most recent values in a circular buffer. */
    seq_t buf[4] = { (seq_t)0, (seq_t)0, (seq_t)3, (seq_t)1 };

    /* Compute successive terms up to n using the recurrence. */
    for (idx_t i = 4u; i <= n; ++i) {
        /* Index positions modulo 4. */
        idx_t cur  =  i      & 3u;
        idx_t val2 = (i - 2u) & 3u;
        idx_t val4 = (i - 4u) & 3u;

        /* Apply the linear recurrence.  The result is guaranteed to be
         * non‑negative, so unsigned overflow will not occur for valid
         * inputs that fit into seq_t. */
        buf[cur] = (seq_t)((((seq_t)2) * buf[val2]) - buf[val4]);
    }

    /* Return the value corresponding to the requested index. */
    return buf[n & 3u];
}
