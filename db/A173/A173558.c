/*
 * A173558  a(n) is the smallest number whose factorial has 10^n trailing
 *          zeros.
 * https://oeis.org/A173558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest integer m such that the factorial m! has exactly
 *            10^n trailing zeros (i.e. the number of trailing zeros of m!
 *            equals 10^n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   mul_small, pow10, zeros_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/mul_small.h"
#include "../utils/zeros_in_factorial.h"

seq_t a(idx_t n)
{
    /* target number of trailing zeros */
    seq_t target = pow10(n);

    /* high bound = 5 * 10^n guarantees enough zeros */
    seq_t high = mul_small(target, (unsigned int)5);
    seq_t low = (seq_t)0;

    while (low < high) {
        seq_t mid = (low + high) / (seq_t)2;

        seq_t z = zeros_in_factorial((idx_t)mid);
        if (z >= target)
            high = mid;
        else
            low = mid + (seq_t)1;
    }

    return low;
}
