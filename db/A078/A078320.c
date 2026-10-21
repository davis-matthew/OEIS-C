/*
 * A078320  Sum of all prime factors of n*rad(n)+1, where rad = A007947
 *          (squarefree kernel).
 * https://oeis.org/A078320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of prime factors of n*rad(n)+1, where rad is A007947
 * Range:     u8 n <= 25, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/rad.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* rad(n) returns the squarefree kernel as seq_t */
    seq_t radn = rad(n);
    uint64_t n64 = (uint64_t)n;
    uint64_t r64 = (uint64_t)radn;

    uint64_t m = n64 * r64 + 1;          /* the number to factor */

    /* Temporary arrays for factor_u64; size 64 is more than enough for m <= 1e8+1 */
    uint64_t ps[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, ps, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (uint64_t j = 0; j < exps[i]; ++j) {
            sum += (seq_t)ps[i];
        }
    }

    return sum;
}
