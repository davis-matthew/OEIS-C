/*
 * A070733  Size of largest conjugacy class in A_n, the alternating group on n
 *          symbols.
 * https://oeis.org/A070733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = n!/(2(n-2)) if n is even, * a(n) = n!/(2(n-3)) if n is
 *            odd * for n>5; a(1)=1, a(2)=1, a(3)=1, a(4)=4, a(5)=20. * The
 *            implementation uses algebraic simplifications that avoid *
 *            intermediate overflow when seq_t is a fixed‑width unsigned type,
 *            * while still working with arbitrary‑precision...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   fact_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fact_upto.h"

seq_t a(idx_t n)
{
    /* small values handled explicitly */
    if (n <= 5) {
        static const seq_t small[] = {
            0ULL,    /* unused for index 0 */
            1ULL,    /* n = 1 */
            1ULL,    /* n = 2 */
            1ULL,    /* n = 3 */
            4ULL,    /* n = 4 */
            20ULL    /* n = 5 */
        };
        return small[n];
    }

    if ((n & 1ULL) == 0) {
        /* even: a(n) = (n*(n-1)/2) * (n-3)! */
        seq_t fact = fact_upto(n - 3);
        seq_t mult = ((seq_t)n * (n - 1)) / 2;
        return fact * mult;
    } else {
        /* odd : a(n) = (n*(n-1)*(n-2)/2) * (n-4)! */
        seq_t fact = fact_upto(n - 4);
        seq_t mult = ((seq_t)n * (n - 1) * (n - 2)) / 2;
        return fact * mult;
    }
}
