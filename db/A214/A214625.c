/*
 * A214625  Let n=r_1*r_2*...*r_k is Fermi-Dirac factorization of n (see
 *          comment). Set g(n) = n + k - 1 and g_i, i>=0 (g_0(n) = n, g_1=g),
 *          is i-th iteration of g. a(n) is the minimal i such that g_i(n) is
 *          in A050376.
 * https://oeis.org/A214625
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal i ≥ 0 such that g_i(n) is a number of the form *
 *            p^(2^k) (prime power with exponent a power of two). Here * *
 *            g(n) = n + (number of distinct prime‐power factors in the *
 *            binary‐decomposition of n) – 1 * * The function iteratively
 *            applies g until the number is of the above form * an...
 * Range:     u8 n <= 265, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static bool primes_ready = false;

static void initialize_primes(void)
{
    if (!primes_ready) {
        build_primes_upto(1000000ULL, &prime_list, &prime_cnt);
        primes_ready = true;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)          /* behaviour below offset is irrelevant */
        return (seq_t)0;

    initialize_primes();

    seq_t cur = (seq_t)n;
    size_t step = 0;

    for (;;) {
        /* factor cur using 64‑bit trial division; cur is expected */
        uint64_t temp = (uint64_t)cur;
        size_t k = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = (uint64_t)prime_list[i];
            if (p > temp / p)     /* stop when p^2 > temp */
                break;
            if (temp % p == 0) {
                uint32_t e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                k += __builtin_popcount((unsigned)e);
            }
        }
        if (temp > 1)          /* remaining prime factor */
            k += 1;

        if (k == 1)            /* n is of the form p^(2^m) */
            return (seq_t)step;

        cur += (seq_t)(k - 1);
        ++step;
    }
}
