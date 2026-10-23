/*
 * A047402  Numbers that are congruent to {2, 3, 6} mod 8.
 * https://oeis.org/A047402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = {8*((n+2)/3)-6 if n%3==1, 8*((n+1)/3)-5 if n%3==2,
 *            8*(n/3)-2 if n%3==0}
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence a(n)=a(n-3)+8 with a(1)=2, a(2)=3, a(3)=6. */
seq_t a(idx_t n)
{
    /* n is 1‑based index */
    idx_t r = n % 3;
    if (r == 0) {                     /* n = 3k  -> a(n) = 8k - 2 */
        seq_t k = (seq_t)(n / 3);
        return (seq_t)8 * k - (seq_t)2;
    } else if (r == 1) { /* n = 3k-2 -> a(n) = 8k - 6 */
        seq_t k = (seq_t)((n + 2) / 3);
        return (seq_t)8 * k - (seq_t)6;
    } else { /* r == 2; n = 3k-1 -> a(n) = 8k - 5 */
        seq_t k = (seq_t)((n + 1) / 3);
        return (seq_t)8 * k - (seq_t)5;
    }
}
