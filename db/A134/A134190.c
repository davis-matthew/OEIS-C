/*
 * A134190  a(1)=1. a(n) = n*a(n-1) if n*a(n-1) has a fewer number of divisors
 *          than n+a(n-1) does. a(n) = n+a(n-1) if n*a(n-1) has a greater or
 *          equal number of divisors than n+a(n-1) does.
 * https://oeis.org/A134190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) if n*a(n-1) has fewer divisors than n+a(n-1), *
 *            else a(n) = n+a(n-1). a(1)=1. * * The function works for any
 *            seq_t supported by the build: * - 8,16,32,64,128 bit unsigned
 *            integers * - arbitrary‑precision integer class * It computes
 *            divisor counts by trial division using floor_sqrt...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* for bool */
#include <stddef.h>           /* for size_t */

static seq_t divisor_count(seq_t n)
/* Return the number of positive divisors of an arbitrary seq_t value.
 * The routine uses trial division up to floor_sqrt(n).  It assumes
 * that floor_sqrt(seq_t) and the operators % and / are available
 * for the seq_t type.  It works for all sizes supported by the build. */
{
    if (n == (seq_t)0) return (seq_t)0;
    if (n == (seq_t)1) return (seq_t)1;

    seq_t temp = n;
    seq_t result = (seq_t)1;

    for (seq_t p = (seq_t)2; p * p <= temp; ++p) {
        if (temp % p == (seq_t)0) {
            int exp = 0;
            while (temp % p == (seq_t)0) {
                temp /= p;
                exp++;
            }
            result = result * (seq_t)(exp + 1);
        }
    }
    if (temp > (seq_t)1) {
        result = result * (seq_t)2;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Base case */
    if (n == (idx_t)1) return (seq_t)1;

    seq_t prev = (seq_t)1;    /* a(1) */
    idx_t i;

    /* Maximum representable value for seq_t */
    seq_t max_val = ~(seq_t)0;
    seq_t INF     = ~(seq_t)0;   /* used as “infinite” divisor count */

    for (i = (idx_t)2; i <= n; ++i) {
        seq_t sum = (seq_t)i + prev;

        /* Detect overflow of sum */
        bool sum_overflow = false;
        if (sizeof(seq_t) <= 8) {
            __uint128_t sum128 = ( __uint128_t)prev + i;
            if (sum128 > max_val) sum_overflow = true;
        }
        /* For larger seq_t types, no overflow */

        /* Compute divisor count for sum if it doesn't overflow */
        seq_t d_sum = INF;
        if (!sum_overflow) {
            d_sum = divisor_count(sum);
        }

        /* Compute product and detect overflow */
        seq_t prod;
        bool prod_overflow = false;
        if (sizeof(seq_t) <= 8) {
            __uint128_t prod128 = ( __uint128_t)prev * i;
            if (prod128 > max_val) prod_overflow = true;
            else prod = (seq_t)prod128;
        } else {
            prod = prev * (seq_t)i;
        }

        seq_t d_prod = INF;
        if (!prod_overflow) {
            d_prod = divisor_count(prod);
        }

        /* Choose the candidate with the smaller number of divisors.
         * In case of equality, the product is chosen only if it has
         * strictly fewer divisors; otherwise the sum is taken. */
        if (!prod_overflow && !sum_overflow && d_prod < d_sum) {
            prev = prod;
        } else {
            prev = sum;
        }
    }

    return prev;
}
