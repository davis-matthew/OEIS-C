/*
 * A098955  Numbers with property that the last digit is the length of the
 *          number (written in base 10).
 * https://oeis.org/A098955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini
 * Formula:   a(1) = 1; for n > 1, let d be the number of digits of a(n), then
 *            a(n) = 10*(n-1) + d. For these n the relation reduces to a(n) =
 *            10*(n-1) + floor(log10(n-1)) + 2
 * Range:     u8 n <= 26, u16 n <= 6554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Determine the digit length d of a(n).  For n >= 2 it satisfies
       10^(d-2)+1 <= n <= 10^(d-1).  We compute d by iterating powers
       of ten.  This loop executes at most 8 times (since the sequence
       stops at n = 10^8). */
    idx_t d = 2;          /* the length for n in [2,10] */
    idx_t power = 10;     /* 10^(d-1) for current d */

    while (n > power) {
        power *= 10;
        d++;
    }

    return (seq_t)10 * ((seq_t)n - (seq_t)1) + (seq_t)d;
}
