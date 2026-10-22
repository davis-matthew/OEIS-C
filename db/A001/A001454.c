/*
 * A001454  Number of permutations of length n with longest increasing
 *          subsequence of length 3.
 * https://oeis.org/A001454
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005802(n) - A000108(n), where u(n) = A005802(n)
 *            satisfies (n+2)^2*u(n) = (10*n^2+2*n-3)*u(n-1) -
 *            9*(n-1)^2*u(n-2), u(0) = 1, u(1) = 1, and Catalan C(n) =
 *            C(n-1)*2*(2n-1)/(n+1), C(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 46,
 *            big any n
 * Helpers:   a001454_big_calc, a001454_div, a001454_fixed,
 *            a001454_fixed_calc, a001454_mul, a001454_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a001454_big_calc.h"
#include "../utils/a001454_div.h"
#include "../utils/a001454_fixed.h"
#include "../utils/a001454_fixed_calc.h"
#include "../utils/a001454_mul.h"
#include "../utils/a001454_sub.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (a001454_fixed()) return a001454_fixed_calc(n);
    return a001454_big_calc(n);
}
