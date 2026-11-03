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
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t zeros_in_factorial(idx_t m)
{
    seq_t count = (seq_t)0;
    seq_t t = (seq_t)m;
    while (t > (seq_t)0) {
        t /= (seq_t)5;
        count += t;
    }
    return count;
}

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
