/*
 * A057614  a(n+1) = a(n)/2 if 2|a(n), a(n)/3 if 3|a(n), a(n)/5 if 5|a(n),
 *          a(n)/7 if 7|a(n), otherwise 11*a(n)+1.
 * https://oeis.org/A057614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 17 For n ≥ 0: if 2 | a(n) then a(n+1) = a(n)/2 else if 3
 *            | a(n) then a(n+1) = a(n)/3 else if 5 | a(n) then a(n+1) =
 *            a(n)/5 else if 7 | a(n) then a(n+1) = a(n)/7 else a(n+1) =
 *            11*a(n)+1 The sequence is periodic with period 11
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo the known period. */
    idx_t r = n % 11;
    seq_t val = 17;            /* a(0) */
    for (idx_t i = 0; i < r; ++i)
    {
        if ((val & 1) == 0)          /* divisible by 2 */
            val /= 2;
        else if (val % 3 == 0)       /* divisible by 3 */
            val /= 3;
        else if (val % 5 == 0)       /* divisible by 5 */
            val /= 5;
        else if (val % 7 == 0)       /* divisible by 7 */
            val /= 7;
        else                         /* otherwise */
            val = val * 11 + 1;
    }
    return val;
}
