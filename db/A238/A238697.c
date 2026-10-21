/*
 * A238697  Quotients connected with the Banach matchboxes problem:
 *          Sum_{i=1..prime(n)-7} 2^(i-1)*binomial(i+2,3)/prime(n) (case 3).
 * https://oeis.org/A238697
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..prime(n)-7} 2^(i-1)*binomial(i+2,3)/prime(n)
 *            (case 3)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   add_power_mul, div_by_small, get_prime, mul_small_inplace
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/add_power_mul.h"
#include "../utils/div_by_small.h"
#include "../utils/mul_small_inplace.h"

/*
 * Big‑integer helpers
 * The representation is little endian base 2^64 (least significant word first).
 * All arithmetic fits into 128‑bit temporaries.
 */

#define MAX_WORDS 2000   /* enough for prime(10000) ≈ 104729 → 2^104722 bits */

seq_t a(idx_t n)
{
    uint64_t prime = get_prime(n);

    /* For n=4 (prime 7) the sum is empty → 0 */
    if (prime <= 7) return (seq_t)0;

    idx_t M = (idx_t)prime - 7;          /* upper limit of the sum (inclusive) */

    static uint64_t power[MAX_WORDS];
    static uint64_t sum_arr[MAX_WORDS];
    int len_power = 1;
    int len_sum   = 0;

    power[0] = 1;            /* 2^(i-1) starts as 1 */

    for (idx_t i = 1; i <= M; ++i) {
        /* binomial (i+2 choose 3) = i(i+1)(i+2)/6 */
        __uint128_t t = (__uint128_t)(i + 2) * (i + 1) * i;
        uint64_t binom = (uint64_t)(t / 6);

        add_power_mul(sum_arr, &len_sum, power, len_power, binom);
        mul_small_inplace(power, &len_power, 2);   /* next power of 2 */
    }

    if (len_sum > 0)
        div_by_small(sum_arr, &len_sum, prime);

    /* Convert big integer to seq_t */
    if (len_sum == 0)
        return (seq_t)0;
    else if (len_sum == 1)
        return (seq_t)sum_arr[0];
    else {
        seq_t res = 0;
        for (int i = len_sum - 1; i >= 0; --i)
            res = (res << 64) + (seq_t)sum_arr[i];
        return res;
    }
}
