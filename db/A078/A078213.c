/*
 * A078213  Smallest multiple of n beginning and ending in n and with a digit
 *          sum that is divisible by n.
 * https://oeis.org/A078213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest multiple of n that starts and ends with the *
 *            decimal digits of n and whose digit sum is divisible by n
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   num_digits_uint64, pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_uint64.h"
#include "../utils/pow10_seq.h"

/* gcd for unsigned 64‑bit integers */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* digit sum of an unsigned 64‑bit integer, return as seq_t */
static inline seq_t digit_sum_uint64(uint64_t x)
{
    unsigned long long sum = 0;
    while (x != 0) {
        sum += x % 10;
        x /= 10;
    }
    return (seq_t)sum;   /* fits into seq_t because sum ≤ 9·digits */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1‑based offset: the caller will never pass 0 */
    seq_t sn = digit_sum_uint64((uint64_t)n);

    /* a trivial solution is n itself, when its digit sum is
       already divisible by n.  This handles the cases n = 1,2,3,5,6,
       7,8,9,12,15,18, etc. */
    if ((sn % n) == 0)
        return (seq_t)n;

    /* number of digits of n */
    uint64_t d = num_digits_uint64((uint64_t)n);

    /* compute g = gcd(n, 10^d)  – all use 64‑bit unsigned math */
    uint64_t pow10_d_uint64 = 1;
    for (uint64_t i = 0; i < d; ++i)
        pow10_d_uint64 *= 10ULL;
    uint64_t g = gcd_uint64((uint64_t)n, pow10_d_uint64);
    uint64_t step = ((uint64_t)n) / g;  /* middle must be a multiple of this */

    seq_t pow10_d_seq = pow10_seq(d);   /* 10^d as seq_t */

    /* iterate over the length k (number of middle digits).  Increasing k
       guarantees that the first found candidate is the smallest possible. */
    for (uint64_t k = 0; ; ++k) {
        seq_t limit = pow10_seq(k);     /* 10^k  (upper bound for middle) */
        seq_t middle = 0;               /* current middle value  */

        /* enumerate all middle values that are multiples of 'step' */
        while (middle < limit) {
            seq_t sum_mid = digit_sum_uint64((uint64_t)middle);
            seq_t sum_total = (sn << 1) + sum_mid;  /* 2*digit_sum(n)+digit_sum(middle) */

            if ((sum_total % n) == 0) {
                seq_t pow10_kd = pow10_seq(k + d);
                seq_t candidate =
                    (seq_t)n * pow10_kd + middle * pow10_d_seq + (seq_t)n;
                return candidate;
            }
            middle += step;
        }
        /* no candidate with this k: try next length */
    }
}
