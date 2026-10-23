/*
 * A018356  Divisors of 250.
 * https://oeis.org/A018356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 250: a(n) = nth divisor of 250 in ascending order; a(n)=0 for n>8. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static int compare_seq_t(const void *a, const void *b)
{
    seq_t va = *(const seq_t*)a;
    seq_t vb = *(const seq_t*)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t divisors[8];
    static size_t div_count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Factor 250 = 2^1 * 5^3 */
        uint64_t primes[8];
        uint64_t exps[8];
        size_t cnt = 0;
        factor_u64((uint64_t)250, primes, exps, &cnt);

        /* Generate all divisors */
        divisors[0] = (seq_t)1;
        div_count = 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t emax = exps[i];
            size_t current = div_count;
            for (uint64_t e = 1; e <= emax; ++e) {
                uint64_t p_pow = 1;
                for (uint64_t k = 0; k < e; ++k) p_pow *= p;
                for (size_t j = 0; j < current; ++j) {
                    divisors[div_count++] = divisors[j] * (seq_t)p_pow;
                }
            }
        }

        /* Sort divisors in ascending order */
        qsort(divisors, div_count, sizeof(seq_t), compare_seq_t);
        initialized = true;
    }

    size_t idx = (size_t)(n - 1);
    if (idx < div_count) return divisors[idx];
    return 0;
}
