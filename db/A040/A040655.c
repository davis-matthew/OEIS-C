/*
 * A040655  Continued fraction for sqrt(682).
 * https://oeis.org/A040655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(682); a0 = 26, period is [8,1,2,5,2,5,2,1,8,52] */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)26;

    static const seq_t period_terms[] = {
        (seq_t)8, (seq_t)1, (seq_t)2, (seq_t)5,
        (seq_t)2, (seq_t)5, (seq_t)2, (seq_t)1,
        (seq_t)8, (seq_t)52
    };
    const idx_t period_len = sizeof(period_terms) / sizeof(period_terms[0]);

    idx_t idx = (n - 1) % period_len;
    return period_terms[idx];
}
