/*
 * A021580  Decimal expansion of 1/576.
 * https://oeis.org/A021580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 1/576.  a(0)=0,
   a(1)=0, a(2)=1, a(3)=7, a(4)=3, a(5)=6,
   and a(n)=1 for all n≥6. */

seq_t a(idx_t n)
{
    switch ((idx_t)n) {
        case 0: return (seq_t)0;
        case 1: return (seq_t)0;
        case 2: return (seq_t)1;
        case 3: return (seq_t)7;
        case 4: return (seq_t)3;
        case 5: return (seq_t)6;
        default: return (seq_t)1;
    }
}
