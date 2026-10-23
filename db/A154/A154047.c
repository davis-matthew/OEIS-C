/*
 * A154047  Number of planar triangular n X n X n nonnegative integer grids
 *          with mirror symmetry about one altitude with every similarly
 *          oriented 4 X 4 X 4 subtriangle summing to 5.
 * https://oeis.org/A154047
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   For n < 12 use the pre‑computed values a(4)=42, a(5)=191,
 *            a(6)=108, a(7)=203, a(8)=80, a(9)=166, a(10)=44, a(11)=95. For n
 *            >= 12 the values repeat with period 4: a(n) = 35 if n ≡ 0 or 2
 *            (mod 4) a(n) = 82 if n ≡ 1 (mod 4) a(n) = 88 if n ≡ 3 (mod 4)
 *            (values for n < 4 are not required by the OEIS of...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle indices below the OEIS offset */
    if (n < 4)
        return 0;

    /* Pre‑computed values for 4 ≤ n ≤ 11 */
    switch ((uint64_t)n) {
        case 4:  return 42;
        case 5:  return 191;
        case 6:  return 108;
        case 7:  return 203;
        case 8:  return 80;
        case 9:  return 166;
        case 10: return 44;
        case 11: return 95;
    }

    /* Periodic pattern for n ≥ 12 */
    uint64_t mod = n & 3ULL;      /* n % 4 */
    if (mod == 0 || mod == 2)     return 35;
    if (mod == 1)                 return 82;
    return 88;                    /* mod == 3 */
}
