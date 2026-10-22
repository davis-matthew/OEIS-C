/*
 * A003556  Numbers that are both square and tetrahedral.
 * https://oeis.org/A003556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003556: Numbers that are both square and tetrahedral (proven finite: 0, 1, 4, 19600) */
seq_t a(idx_t n)
{
  switch (n) {
    case 1: return (seq_t)0;
    case 2: return (seq_t)1;
    case 3: return (seq_t)4;
    case 4: return (seq_t)19600;
    default: return (seq_t)0;
  }
}
