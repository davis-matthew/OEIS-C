/*
 * A067461  a(n) = mu(prime(n)+1)+1.
 * https://oeis.org/A067461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = mu(p_n+1)+1 (offset 1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Store primes in a dynamic array.  The array is enlarged only when
 * a larger prime is requested. */
static uint32_t *primes       = NULL;
static size_t   primes_count  = 0;
static size_t   primes_cap    = 0;
static uint32_t last_checked  = 1;

/* Ensure that at least 'need' primes are available in 'primes'.  The
 * sieve is performed with trial division against previously found
 * primes.  This keeps the memory footprint tiny while still being
 * fast enough for n≈10000. */
static void ensure_primes(size_t need)
{
    if (primes_count >= need)
        return;

    /* Grow the array capacity if required. */
    if (primes_cap < need) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 16;
        while (new_cap < need)
            new_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp)
            exit(1);          /* memory exhausted */
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Find new primes by trial division. */
    uint32_t candidate = last_checked + 1;
    while (primes_count < need) {
        bool is_prime = true;
        uint32_t r = (uint32_t)sqrt((double)candidate);
        for (size_t i = 0; i < primes_count && primes[i] <= r; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[primes_count++] = candidate;
        }
        last_checked = candidate;
        ++candidate;
    }
}

/* Compute a(n) for n >= 1.  If n == 0 the function returns 0
 * (only used for safety; the OEIS offset is 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Obtain the n-th prime (1‑based). */
    ensure_primes((size_t)n);
    uint64_t p = (uint64_t)primes[n - 1];
    uint64_t x = p + 1;          /* number whose μ value is needed */

    /* Compute Möbius μ(x).  μ(x) = 0 if x is not square‑free,
     * μ(x) = 1 if x has an even number of distinct prime factors,
     * μ(x) = -1 otherwise.
     */
    int mu;
    if (x == 1) {
        mu = 1;
    } else {
        uint64_t temp = x;
        int cnt = 0;
        for (size_t i = 0; i < primes_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
            uint32_t pf = primes[i];
            if (temp % pf == 0) {
                ++cnt;
                temp /= pf;
                if (temp % pf == 0) {       /* square factor detected */
                    mu = 0;
                    goto finish_mu;
                }
                while (temp % pf == 0)
                    temp /= pf;
            }
        }
        if (temp > 1)
            ++cnt;
        mu = (cnt & 1) ? -1 : 1;
    }

finish_mu:
    /* a(n) = μ(p_n+1) + 1 */
    return (seq_t)(mu + 1);
}
