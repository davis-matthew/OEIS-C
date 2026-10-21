/*
 * A060192  Union_i p(4i+2), p(4i+3), where p(k) = k-th prime.
 * https://oeis.org/A060192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(4k+2) or p(4k+3), where p(m) is the m‑th prime (indices
 *            start at 1). The function generates primes incrementally and
 *            stores the required terms
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *terms    = NULL;
static size_t terms_len = 0, terms_cap = 0;

static uint32_t *primes    = NULL;
static size_t primes_len = 0, primes_cap = 0;

static idx_t prime_index = 0;   /* current prime index (1‑based) */
static uint32_t candidate = 2;  /* next integer to test for primality */

static void ensure_terms(idx_t n)
{
    while (terms_len <= (size_t)n) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            ++prime_index;
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap << 1 : 8;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = candidate;

            if ((prime_index & 3) == 2 || (prime_index & 3) == 3) {
                if (terms_len == terms_cap) {
                    terms_cap = terms_cap ? terms_cap << 1 : 4;
                    terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)candidate;
            }
        }

        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;  /* skip even numbers > 2 */
    }
}

seq_t a(idx_t n)
{
    ensure_terms(n);
    return terms[(size_t)n];
}
