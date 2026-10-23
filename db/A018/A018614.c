/*
 * A018614  Divisors of 728.
 * https://oeis.org/A018614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Divisors of 728 in ascending order.
    a(n) returns the n‑th divisor of 728, a(1)=1, a(16)=728.
    For n>16 the function returns 0.  */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Constant whose divisors we want */
    const uint64_t target = 728ULL;

    /* Collect prime factors and exponents of target */
    uint64_t primes[8];
    int exps[8];
    size_t pf_cnt = 0;
    uint64_t tmp = target;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt]   = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {
        primes[pf_cnt] = tmp;
        exps[pf_cnt]   = 1;
        ++pf_cnt;
    }

    /* Generate all divisors (at most 16 for 728) */
    seq_t divisors[64];
    size_t div_cnt = 1;
    divisors[0] = (seq_t)1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        /* Compute successive powers of primes[i] */
        seq_t p_pow = (seq_t)1;
        size_t old_cnt = div_cnt;
        for (int e = 1; e <= exps[i]; ++e) {
            p_pow *= (seq_t)primes[i];
            for (size_t j = 0; j < old_cnt; ++j) {
                divisors[div_cnt++] = divisors[j] * p_pow;
            }
        }
    }

    /* Simple insertion sort for at most 16 elements */
    for (size_t i = 1; i < div_cnt; ++i) {
        seq_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j - 1] > key) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }

    /* Return the n-th divisor if within range, else 0 */
    if (n == 0 || n > div_cnt) {
        return (seq_t)0;
    }
    return divisors[n - 1];
}
