/*
 * A215947  Difference between the sum of the even divisors and the sum of the
 *          odd divisors of 2n.
 * https://oeis.org/A215947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, mult
 * Formula:   a(n) = 4*sigma(n) - sigma(2*n) (sigma = sum of divisors)
 * Range:     u8 n <= 59, u16 n <= 9239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* ---------- Static prime cache ---------- */
static uint64_t *prime_list = NULL;
static size_t    prime_cnt   = 0;
static bool      primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    /* 65536 is more than enough for the tests (n ≤ 10 000) */
    build_primes_upto(65536, &prime_list, &prime_cnt);
    primes_ready = true;
}

/* ---------- Sum of divisors for a 64‑bit integer ---------- */
static seq_t sigma_of_uint64(uint64_t n)
{
    if (n == 0) return 0;   /* undefined, but should not occur */

    seq_t result = 1;
    uint64_t m = n;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= m; i++)
    {
        uint64_t p = prime_list[i];
        if (m % p == 0)
        {
            seq_t term = 1;          /* 1 + p + p^2 + … + p^e */
            seq_t power = 1;
            do
            {
                m /= p;
                power *= (seq_t)p;
                term += power;
            } while (m % p == 0);
            result *= term;
        }
    }

    if (m > 1)         /* remaining prime factor */
    {
        result *= ((seq_t)m + 1);
    }

    return result;
}

/* ---------- Sequence a(n) ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Out of domain, return 0 */

    init_primes();

    seq_t sigma_n = sigma_of_uint64((uint64_t)n);

    /* extract exponent of 2 in n */
    uint64_t tmp = (uint64_t)n;
    unsigned e = 0;
    while ((tmp & 1ULL) == 0)
    {
        tmp >>= 1;
        ++e;
    }

    /* sigma(2n) = sigma(n) * (2^(e+2)-1) / (2^(e+1)-1)   */
    seq_t term_e     = (((seq_t)1) << (e + 1)) - 1;
    seq_t term_e_plus = (((seq_t)1) << (e + 2)) - 1;
    seq_t sigma_2n = sigma_n * term_e_plus / term_e;

    seq_t result = ((seq_t)4) * sigma_n - sigma_2n;
    return result;
}
