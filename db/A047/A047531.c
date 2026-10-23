/*
 * A047531  Numbers that are congruent to {2, 3, 7} mod 8.
 * https://oeis.org/A047531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n%3 == 0 : 8*(n/3) - 1 n%3 == 1 : 8*((n+2)/3) - 6 n%3 ==
 *            2 : 8*((n+1)/3) - 5
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* values for n < 1 are undefined */
    idx_t r = n % 3;
    idx_t k;
    seq_t res;
    if (r == 0) {                 /* n = 3k */
        k = n / 3;
        res = (seq_t)8 * (seq_t)k - (seq_t)1;
    } else if (r == 1) {          /* n = 3k-2  → k = (n+2)/3 */
        k = (n + 2) / 3;
        res = (seq_t)8 * (seq_t)k - (seq_t)6;
    } else {                      /* r == 2, n = 3k-1  → k = (n+1)/3 */
        k = (n + 1) / 3;
        res = (seq_t)8 * (seq_t)k - (seq_t)5;
    }
    return res;
}
