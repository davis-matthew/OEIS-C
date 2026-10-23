/*
 * A156797  Numbers k such that k^2 + 2 == 0 (mod (9^2)).
 * https://oeis.org/A156797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1)=22+81*(k-1), a(2k)=59+81*(k-1)
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* below offset – behaviour irrelevant */
        return (seq_t)0;

    if (n & 1) {                 /* n odd: 2k-1 */
        seq_t k = ((seq_t)n + 1) / 2;
        return (seq_t)22 + ((seq_t)81 * (k - 1));
    } else {                     /* n even: 2k */
        seq_t k = (seq_t)n / 2;
        return (seq_t)59 + ((seq_t)81 * (k - 1));
    }
}
