/*
 * A025842  Expansion of 1/((1-x^3)*(1-x^6)*(1-x^8)).
 * https://oeis.org/A025842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{c=0}^{⌊n/8⌋} [(n-8c) mod 3 == 0 ? floor((n-8c)/6)+1
 *            : 0]
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             (number of partitions of n into parts 3, 6, and 8) */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    for (idx_t c = (idx_t)0; c <= n / (idx_t)8; ++c)
    {
        idx_t rem = n - (idx_t)8 * c;
        /* rem >= 0 because c <= n/8 */
        if (rem % (idx_t)3 == (idx_t)0)
        {
            /* floor(rem / 6) + 1 */
            seq_t add = (seq_t)(rem / (idx_t)6) + (seq_t)1;
            result += add;
        }
    }
    return result;
}
