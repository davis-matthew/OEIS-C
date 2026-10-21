/*
 * A056009  a(n) = (n^n + 1)/ (n^(2^a) + 1), where 2^a is the highest power of
 *          2 dividing n.
 * https://oeis.org/A056009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^n + 1)/(n^(2^a) + 1) where 2^a is the highest power of
 *            2 dividing n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   count_trailing_zeros_u64, pow_seq_internal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_trailing_zeros_u64.h"
#include "../utils/pow_seq_internal.h"

seq_t a(idx_t n)
{
    /* Find the exponent a such that 2^a divides n. */
    int a_pow = count_trailing_zeros_u64((unsigned long long)n);   /* a_pow = v2(n) */
    idx_t k = 1ULL << a_pow;                /* k = 2^a */
    idx_t t = n / k;                        /* odd integer, n = k * t */

    /* Compute n^k and the starting power n^(k*(t-1)). */
    seq_t base_k = pow_seq_internal((seq_t)n, k);          /* n^k */
    seq_t sum = (seq_t)0;
    bool add = true;                                        /* start with plus */

    /* Start with the largest term n^(k*(t-1)). */
    seq_t current = pow_seq_internal(base_k, t - 1);        /* (n^k)^(t-1) */

    for (idx_t i = 0; i < t; ++i) {
        if (add)
            sum += current;
        else
            sum -= current;

        add = !add;

        if (i + 1 < t)
            current /= base_k;    /* next term: divide by n^k */
    }

    return sum;
}
