/*
 * A166383  Let dsf(n) = n_1^{n_1}+n_2^{n_2}+n_3^{n_3} + n_m^{n_m}, where
 *          {n_1,n_2,n_3,...n_m} is the list of the digits of an integer n.
 *          dsf(1583236420) =1682731 and dsf(1682731) =
 *          18470991,...,dsf(388290999) = 1583236420,.. in this way this 97
 *          numbers make a loop for the function dsf. In fact this is the l...
 * https://oeis.org/A166383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_digits.h"

/* Computes the A166383 sequence by iterating the dsf function starting
   from 1583236420. */

/* d^d for d = 0..9; 0^0 is defined as 1. */
static const seq_t digit_pow[10] = {
    1ull,          /* 0^0 */
    1ull,          /* 1^1 */
    4ull,          /* 2^2 */
    27ull,         /* 3^3 */
    256ull,        /* 4^4 */
    3125ull,       /* 5^5 */
    46656ull,      /* 6^6 */
    823543ull,     /* 7^7 */
    16777216ull,   /* 8^8 */
    387420489ull   /* 9^9 */
};

/* Sum of d^d over the decimal digits of x. */
static inline seq_t dsf(seq_t x)
{
    uint8_t buf[30];
    size_t len;
    get_digits(x, buf, &len);

    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        sum += digit_pow[buf[i]];
    }
    return sum;
}

/* a(n) = nth element of the 97‑term dsf cycle beginning with
   1583236420. */
seq_t a(idx_t n)
{
    static seq_t cycle[200];
    static size_t cycle_len = 0;
    static bool initialized = false;

    if (!initialized) {
        const seq_t start = 1583236420;
        size_t i = 0;
        seq_t prev = start;
        cycle[i++] = prev;         /* a(1) */

        while (1) {
            seq_t cur = dsf(prev);
            if (cur == start)        /* cycle closed */
                break;
            cycle[i++] = cur;
            prev = cur;
        }
        cycle_len = i;              /* should be 97 */
        initialized = true;
    }

    /*  n ≥ 1 → index = (n-1) mod cycle_len  */
    idx_t idx = (n - 1) % (idx_t)cycle_len;
    return cycle[idx];
}
