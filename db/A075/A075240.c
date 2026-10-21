/*
 * A075240  Smallest prime whose base-n reversal is not a prime.
 * https://oeis.org/A075240
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   A075240(n) = smallest prime p ≥ 2 such that the base‑n reversal
 *            of p is not prime. For each n ≥ 2 this function returns that
 *            prime (as a seq_t). The computation uses trial division with a
 *            small pre‑computed list of primes (up to 100 000) which is
 *            sufficient for the required range of n (≤ 10 000)....
 * Range:     u8 n <= 235, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_in_base_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_in_base_uint64.h"

/*--- Prime sieve -------------------------------------------------------*/
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;

static void sieve_primes_up_to(uint64_t limit)
{
    if (prime_cnt > 0) return;                 /* already done */
    bool *sieve = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!sieve) return;                       /* allocation failure */

    for (size_t i = 0; i <= limit; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) { free(sieve); return; }
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) prime_list[idx++] = i;

    prime_cnt = cnt;
    free(sieve);
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/*--- Main sequence function --------------------------------------------*/
extern seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* undefined for indices < 2 */

    /* initialise the prime table once */
    static bool init = false;
    if (!init) {
        sieve_primes_up_to(100000);          /* enough for n <= 10 000 */
        init = true;
    }

    uint64_t base = n;
    uint64_t p = 2;

    while (true) {
        if (is_prime_uint64(p)) {
            uint64_t rev = reverse_in_base_uint64(p, base);
            if (!is_prime_uint64(rev)) {
                return (seq_t)p;
            }
        }
        /* advance to the next prime candidate: 2 → 3 → 5 → 7 … */
        p = (p == 2) ? 3 : p + 2;
    }
}
