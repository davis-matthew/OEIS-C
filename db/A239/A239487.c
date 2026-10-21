/*
 * A239487  Maximum number of diameters of an indecomposable algebraic curve
 *          of degree n.
 * https://oeis.org/A239487
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is odd; a(n) = n + 2 if n is even, except a(6) =
 *            12, a(8) = 18, a(12) = a(20) = a(24) = 30
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Index offset is 3; behaviour for n < 3 is irrelevant. */
    if (n & 1ULL)          /* odd */
        return (seq_t)n;

    switch ((idx_t)n) {
    case 6:  return (seq_t)12;      /* special case */
    case 8:  return (seq_t)18;      /* special case */
    case 12:
    case 20:
    case 24: return (seq_t)30;      /* special cases */
    default: return (seq_t)n + (seq_t)2;
    }
}
