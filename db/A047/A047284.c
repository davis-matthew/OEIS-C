/*
 * A047284  Numbers that are congruent to {2, 3, 6} mod 7.
 * https://oeis.org/A047284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = { 7*floor(n/3)-1 , if n mod 3 = 0; 7*(floor(n/3)+1)-5 ,
 *            if n mod 3 = 1; 7*(floor(n/3)+1)-4 , if n mod 3 = 2; }
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k;
    switch (n % 3)
    {
    case 0:                 /* n = 3k */
        k = n / 3;
        return (seq_t)(7 * k) - 1;
    case 1:                 /* n = 3k - 2 */
        k = n / 3 + 1;
        return (seq_t)(7 * k) - 5;
    case 2:                 /* n = 3k - 1 */
        k = n / 3 + 1;
        return (seq_t)(7 * k) - 4;
    }
    return 0; /* unreachable for n ≥ 1 */
}
