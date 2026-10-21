/*
 * A082593  Values in Pfennigs of German money before the introduction of the
 *          Euro.
 * https://oeis.org/A082593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1,2,5,10,50 for n=1..5; for n>=6 let k=(n-6)/3+1,
 *            r=(n-6)%3; a(n) = 10^k * [10,20,50][r]
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* out of range, not used */
    if (n <= 5) {
        static const seq_t init[5] = {
            (seq_t)1,
            (seq_t)2,
            (seq_t)5,
            (seq_t)10,
            (seq_t)50
        };
        return init[n - 1];
    }
    idx_t offset = n - 6;
    idx_t k = offset / 3 + 1;
    unsigned int r = offset % 3;
    static const unsigned int mult[3] = {10, 20, 50};
    seq_t p10 = pow10(k);
    return mul_small(p10, mult[r]);
}
