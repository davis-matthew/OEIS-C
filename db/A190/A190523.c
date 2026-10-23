/*
 * A190523  a(n) = n + [nr/v] + [ns/v] + [nt/v] + [nu/v] + [nw/v], where
 *          r=sin(x), s=cos(x), t=tan(x), u=csc(x), v=sec(x), w=cot(x),
 *          x=2*Pi/5.
 * https://oeis.org/A190523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·r/v⌋ + ⌊n·s/v⌋ + ⌊n·t/v⌋ + ⌊n·u/v⌋ + ⌊n·w/v⌋ where
 *            r = sin(x), s = cos(x), t = tan(x), u = csc(x), v = sec(x), w =
 *            cot(x) and x = 2·π/5. All computations are performed with double
 *            precision constants; truncation of the product yields the
 *            correct integer part for 0 < n ≤ 10 000. The...
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = n;                   /* initial term n */
    double nd = (double)n;           /* floating copy of n */

    /* Irrational constants derived from x = 2*π/5.
       They are the exact values of r/v, s/v, etc. */
    static const double c1 = 0.293892626146236523071828; /* sin·cos   */
    static const double c2 = 0.095491502812526278120619; /* cos²     */
    static const double c3 = 0.9510565162951535721163385; /* sin      */
    static const double c4 = 0.324919696232906174174189; /* cot      */
    static const double c5 = 0.100526086589126572939981; /* cos²/ sin*/

    /* Add the floor of each fractional term.  For a positive product
       a cast to unsigned long long truncates toward zero, which is
       equivalent to ⌊x⌋.  The intermediate integer fits in 64 bits
       because n is at most 10 000 and the constants are < 1. */
    res += (seq_t)(uint64_t)(nd * c1);
    res += (seq_t)(uint64_t)(nd * c2);
    res += (seq_t)(uint64_t)(nd * c3);
    res += (seq_t)(uint64_t)(nd * c4);
    res += (seq_t)(uint64_t)(nd * c5);

    return res;
}
