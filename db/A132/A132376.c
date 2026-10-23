/*
 * A132376  Algorithmic drum function based on two bar 16 notes per bar: Funk
 *          drum : Based on Andy Newark "In Time" from Sly Stone's "Fresh"
 *          1973.
 * https://oeis.org/A132376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Mapping of n mod 32 to drum choice values:
   0,3,5,9,11,13,18,23,26,27 → 2
   6,14,15,16,21,30,31       → 3
   19                        → 4
   20                        → 1
   otherwise                 → 5
   (sequence offset 1) */
seq_t a(idx_t n)
{
    static const seq_t tbl[32] = {
        2, 5, 5, 2, 5, 2, 3, 5, 5, 2, 5, 2, 5, 2, 3, 3,
        3, 5, 2, 4, 1, 3, 5, 2, 5, 5, 2, 2, 5, 5, 3, 3
    };
    return tbl[(unsigned)(n & 31U)];
}
