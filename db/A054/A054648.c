/*
 * A054648  Number of labeled pure 2-complexes on n nodes (0-simplexes) with 4
 *          2-simplexes and 11 1-simplexes.
 * https://oeis.org/A054648
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            360*C(n,6)+10710*C(n,7)+42000*C(n,8)+41580*C(n,9)+12600*C(n,10)
 * Range:     u8 none, u16 n <= 7, u32 n <= 17, u64 n <= 150, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;
    seq_t res = 0;
    res += (seq_t)360   * binom(n, 6);
    res += (seq_t)10710 * binom(n, 7);
    res += (seq_t)42000 * binom(n, 8);
    res += (seq_t)41580 * binom(n, 9);
    res += (seq_t)12600 * binom(n, 10);
    return res;
}
