/*
 * A099745  Number of permutations of [n] with exactly 2 valleys which avoid
 *          the permutation 1324.
 * https://oeis.org/A099745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 34, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A099745: Number of permutations of [n] with exactly 2 valleys which avoid the permutation 1324
   Recurrence: a(n) = 20 a(n-1) – 174 a(n-2) + 860 a(n-3) – 2641 a(n-4) + 5160 a(n-5) – 6264 a(n-6) + 4320 a(n-7) – 1296 a(n-8)
   (derived from the generating function x⁵(12–72x+137x²–81x³)/((1–3x)⁴(1–2x)⁴))
   Initial values (a(1) … a(8) ): 0, 0, 0, 0, 12, 168, 1409, 9187.
*/

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) corresponds to n == 1. */
    static const seq_t init[9] = {0, 0, 0, 0, 0, 12, 168, 1409, 9187};

    if (n <= 8)
        return init[n];

    /* Keep the last eight known terms; vals[0] = a(n-7), vals[7] = a(n). */
    seq_t vals[8];
    for (int i = 0; i < 8; ++i)
        vals[i] = init[i + 1];   /* init[1] … init[8] */

    for (idx_t k = 9; k <= n; ++k)
    {
        /* Compute the recurrence without negative intermediate values:
           Positive part P = 20 a_{k-1} + 860 a_{k-3} + 5160 a_{k-5} + 4320 a_{k-7}
           Negative part Q = 174 a_{k-2} + 2641 a_{k-4} + 6264 a_{k-6} + 1296 a_{k-8}
           Result = P - Q  (P >= Q for all k >= 9) */
        seq_t p = vals[7] * (seq_t)20;
        p += vals[5] * (seq_t)860;
        p += vals[3] * (seq_t)5160;
        p += vals[1] * (seq_t)4320;

        seq_t q = vals[6] * (seq_t)174;
        q += vals[4] * (seq_t)2641;
        q += vals[2] * (seq_t)6264;
        q += vals[0] * (seq_t)1296;

        seq_t next = p - q;  /* safe: next >= 0 */

        /* Shift values left to make room for the new term */
        for (int i = 0; i < 7; ++i)
            vals[i] = vals[i + 1];
        vals[7] = next;
    }

    return vals[7];
}
