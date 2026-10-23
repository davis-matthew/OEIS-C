/*
 * A177964  Indices m for which A177961(m) = 4.
 * https://oeis.org/A177964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 15·⌊(n-1)/2⌋ if n is odd, * a(n) = 13 + 15·⌊(n-1)/2⌋
 *            otherwise. * This generates the sequence 2,13,17,28,32,43,47,…
 *            (indices m for which A177961(m)=4)
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so the caller must not pass n = 0. */
    seq_t half = (n - 1) / 2;          /* floor((n-1)/2) */
    if (n & 1) {                       /* odd n */
        return (seq_t)2 + (seq_t)15 * half;
    } else {                           /* even n */
        return (seq_t)13 + (seq_t)15 * half;
    }
}
