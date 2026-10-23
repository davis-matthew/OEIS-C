/*
 * A047465  Numbers that are congruent to {1, 2, 4} mod 8.
 * https://oeis.org/A047465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 8k - 4 * a(3k-1) = 8k - 6 * a(3k-2) = 8k - 7 * (k >= 1)
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    idx_t mod = n % 3;
    idx_t k;                       /*  k fits in idx_t  */
    seq_t result;

    switch (mod) {
    case 1:                         /* n = 3k-2  */
        k = (n + 2) / 3;            /* k = ceil(n/3)   */
        result = (seq_t)8 * (seq_t)k - 7;
        break;
    case 2:                         /* n = 3k-1  */
        k = (n + 1) / 3;            /* k = floor((n+1)/3)  */
        result = (seq_t)8 * (seq_t)k - 6;
        break;
    case 0:                         /* n = 3k    */
        k = n / 3;
        result = (seq_t)8 * (seq_t)k - 4;
        break;
    default:
        result = 0;                 /* unreachable */
    }

    return result;
}
