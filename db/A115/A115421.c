/*
 * A115421  Numbers having a 1 in position 5 of their binary expansion.
 * https://oeis.org/A115421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 32 + (n-1) + 32 * floor((n-1)/32); The sequence consists
 *            of all integers whose 5th binary bit (2^5) is set, i.e. numbers
 *            congruent to 32‑63 modulo 64
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1. */
    idx_t k = n - 1;                 /* offset from first term */
    seq_t kseq = (seq_t)k;           /* promote to seq_t for arithmetic */
    seq_t term = 32 + kseq + ((kseq / 32) * 32);
    return term;
}
