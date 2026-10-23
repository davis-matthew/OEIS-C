/*
 * A100688  a(n) = prime(n) * 3^prime(n) - 1.
 * https://oeis.org/A100688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * 3^prime(n) - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

/* incremental storage of prime numbers */
static uint64_t *primes      = NULL;
static idx_t    primes_count = 0;
static idx_t    primes_cap   = 0;
static uint64_t current_candidate = 2;

static void ensure_primes(idx_t n)
{
    while (primes_count < n)
    {
        uint64_t v = current_candidate;
        bool is_prime = v >= 2;

        for (idx_t i = 0; i < primes_count; ++i)
        {
            uint64_t p  = primes[i];
            uint64_t pp = p * p;
            if (pp > v)
                break;
            if (v % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            if (primes_count >= primes_cap)
            {
                primes_cap = (primes_cap ? primes_cap * 2 + 10 : 16);
                uint64_t *tmp = (uint64_t *)realloc(primes,
                        sizeof(uint64_t) * primes_cap);
                if (!tmp)
                    exit(1);         /* memory exhausted */
                primes = tmp;
            }
            primes[primes_count++] = v;
        }
        ++current_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n <= 0 */
    ensure_primes(n);
    uint64_t p       = primes[n - 1];   /* nth prime, 1‑based */
    seq_t pow3       = seq_pow((seq_t)3, (idx_t)p);
    return (seq_t)p * pow3 - 1;
}
