/*
 * A025898  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^9)).
 * https://oeis.org/A025898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 422, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the number of partitions of n into parts 6, 7 and 9
 * by iterating over the number of 9's and counting the solutions
 * of 6a + 7b = n – 9c.  This yields the sequence A025898.
 */
seq_t a(idx_t n)
{
    seq_t total = 0;                             /* accumulated result */
    idx_t max_c = n / ((idx_t)9);                /* max possible number of 9's */

    for (idx_t c = 0; c <= max_c; ++c) {
        idx_t R = n - ((idx_t)9 * c);            /* remaining amount after 9's */

        idx_t q = R / ((idx_t)7);               /* floor(R/7)   */
        idx_t r = R % ((idx_t)6);               /* R mod 6      */

        if (q >= r) {                           /* at least one solution for 6a+7b = R? */
            seq_t add = (seq_t)((q - r) / ((idx_t)6) + (idx_t)1);
            total += add;
        }
    }
    return total;
}
