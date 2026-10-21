/*
 * A226532  If n = Product_{i>0} prime(i)^e(i), then a(n) = Product_{i>0}
 *          prime(i)^(XOR_{j>=i} e(j)), where XOR is bitwise XOR.
 * https://oeis.org/A226532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏prime_i prime(i)^(XOR_{j>=i} e(j))
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where n = ∏prime_i prime(i)^e(i)  (e(i) may be zero for some primes). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

static uint32_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t   prime_lim  = 0;

/* Build the list of all primes <= limit, store in prime_list. */
static void init_prime_list(idx_t limit)
{
    uint64_t *tmp;          /* temporary array returned by build_primes_upto */
    size_t   cnt;
    build_primes_upto(limit, &tmp, &cnt);

    if (prime_list)
        free(prime_list);
    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    for (size_t i = 0; i < cnt; ++i)
        prime_list[i] = (uint32_t)tmp[i];

    free(tmp);
    prime_cnt = cnt;
    prime_lim = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1U)
        return (seq_t)1;

    /* Ensure we have primes up to n for the current call. */
    if (prime_list == NULL || n > prime_lim)
        init_prime_list(n);

    /* Factor n: primes_f contains the prime divisors in ascending order
       and exps_f contains their exponents. */
    uint64_t primes_f[64];
    uint64_t exps_f[64];
    size_t   cnt_f = 0;
    factor_u64((uint64_t)n, primes_f, exps_f, &cnt_f);

    /* The largest prime factor of n. */
    uint64_t p_max = primes_f[cnt_f - 1];

    /* Find index of the largest prime <= p_max in prime_list. */
    size_t pi;
    for (pi = 0; pi < prime_cnt && prime_list[pi] <= p_max; ++pi);
    if (pi == 0)
        pi = 0;
    else
        --pi;

    seq_t   result  = (seq_t)1;
    uint64_t xor_acc = 0;                   /* XOR of exponents of processed primes */
    int      idx_f   = (int)cnt_f - 1;      /* current index in primes_f (descending) */

    /* Iterate over all primes from p_max down to 2. */
    for (long long i = (long long)pi; i >= 0; --i) {
        uint32_t p = prime_list[i];

        /* if p is a factor, incorporate its exponent */
        if (idx_f >= 0 && primes_f[idx_f] == p) {
            xor_acc ^= exps_f[idx_f];
            idx_f--;
        }

        if (xor_acc != 0) {
            result *= big_pow((seq_t)p, (seq_t)xor_acc);
        }
    }

    return result;
}
