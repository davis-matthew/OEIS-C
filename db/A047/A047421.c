/*
 * A047421  Floor(8n/7).
 * https://oeis.org/A047421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊8*n/7⌋
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (alternative): a(n) = a(n-1) + a(n-7) - a(n-8), a(0)=0, a(1)=1, ... */
seq_t a(idx_t n)
{
    /* compute floor(8*n/7) using integer arithmetic */
    return ((seq_t)n * (seq_t)8) / (seq_t)7;
}
