/*
 * A047443  Numbers that are congruent to {3, 5, 6} mod 8.
 * https://oeis.org/A047443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n/3)-2 if n%3==0 * a(n) = 8*((n+2)/3)-5 if n%3==1 *
 *            a(n) = 8*((n+1)/3)-3 if n%3==2 (offset 1)
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based index, n >= 1 */
    idx_t r = n % 3;
    seq_t k, res;
    if (r == 0) {
        k   = (seq_t)(n / 3);
        res = k * (seq_t)8 - (seq_t)2;
    } else if (r == 1) {
        k   = (seq_t)((n + 2) / 3);
        res = k * (seq_t)8 - (seq_t)5;
    } else { /* r == 2 */
        k   = (seq_t)((n + 1) / 3);
        res = k * (seq_t)8 - (seq_t)3;
    }
    return res;
}
