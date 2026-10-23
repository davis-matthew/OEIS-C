/*
 * A047323  Numbers that are congruent to {2, 5, 6} mod 7.
 * https://oeis.org/A047323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n+2)/3) - 5 if n ≡ 1 (mod 3) a(n) = 7*((n+1)/3) - 2
 *            if n ≡ 2 (mod 3) a(n) = 7*( n/3 ) - 1 if n ≡ 0 (mod 3) Offset 1
 *            (a(1) = 2)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;          /* 0,1,2 */
    idx_t k;
    seq_t val;

    if (r == 0) {             /* n = 3k */
        k = n / 3;
        val = (seq_t)7 * (seq_t)k - (seq_t)1;
    } else if (r == 1) {      /* n = 3k - 2 */
        k = (n + 2) / 3;
        val = (seq_t)7 * (seq_t)k - (seq_t)5;
    } else {                  /* r == 2, n = 3k - 1 */
        k = (n + 1) / 3;
        val = (seq_t)7 * (seq_t)k - (seq_t)2;
    }
    return val;
}
