/*
 * A166144  a(n) = (11*n^2 + 11*n - 20)/2.
 * https://oeis.org/A166144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^2 + 11*n - 20)/2 (OEIS A166144)
 * Range:     u8 n <= 4, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or less the behaviour is irrelevant, but return 0. */
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;          /* promote index to seq_t for safety */
    seq_t num = (seq_t)11 * sn * sn + (seq_t)11 * sn - (seq_t)20;
    return num / (seq_t)2;
}
