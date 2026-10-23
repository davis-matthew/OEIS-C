/*
 * A147675  Divide by 2, multiply by 4, repeat.
 * https://oeis.org/A147675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 10; a(2) = 5; a(n) = 2 * a(n-2) (n >= 3)
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 58, u64 n <= 122,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form:
   a(n) = 10 * 2^((n-1)/2)        if n is odd
   a(n) =  5 * 2^(n/2 - 1)        if n is even
*/
seq_t a(idx_t n)
{
    /* offset is 1; assumes n >= 1 */
    if (n & 1) { /* odd */
        idx_t exp = (n - 1) >> 1;   /* (n-1)/2 */
        return 10 * ((seq_t)1 << exp);
    } else { /* even */
        idx_t exp = (n >> 1) - 1;   /* n/2 - 1 */
        return 5 * ((seq_t)1 << exp);
    }
}
