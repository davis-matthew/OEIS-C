/*
 * A051743  a(n) = (1/24)*n*(n + 5)*(n^2 + n + 6).
 * https://oeis.org/A051743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 5)*(n^2 + n + 6)/24
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 270, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. For n < 1
       the behaviour is undefined; we simply return 0. */
    if (n == 0) return 0;

    seq_t sn   = (seq_t)n;               /* convert index to seq_t */
    seq_t n2   = sn * sn;                /* n^2            */
    seq_t part = n2 + sn + 6;            /* n^2 + n + 6    */
    seq_t prod = sn * (sn + 5) * part;   /* n*(n+5)*(n^2+n+6) */

    return prod / 24;                    /* divide by 24 exactly */
}
