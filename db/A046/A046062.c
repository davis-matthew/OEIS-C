/*
 * A046062  Primes of the form n*phi(n)+1 where phi(n) is the Euler function.
 * https://oeis.org/A046062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 94, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046062: primes of the form n*phi(n)+1
   a(n) = the nth prime p such that p = n*phi(n)+1 for some n ≥ 1.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"  /* gives bool is_prime_big(seq_t) */

/*-------------------------------------------------------------
 *  Sieve for Euler's totient up to a fixed limit
 *-------------------------------------------------------------*/
#define MAX_LIMIT 2000000UL  /* sufficient for n≤10000 */

static idx_t *phi_arr = NULL;   /* phi[m] for 0..MAX_LIMIT */
static bool phi_ready = false;

/* initialise phi_arr with a linear sieve */
static void init_phi(void)
{
    if (phi_ready)
        return;
    phi_arr = (idx_t*)malloc((MAX_LIMIT + 1) * sizeof(idx_t));
    if (!phi_arr) abort();

    /* allocate room for the prime list */
    idx_t *primes = (idx_t*)malloc((MAX_LIMIT / 10 + 10) * sizeof(idx_t));
    if (!primes) abort();
    size_t pc = 0;

    phi_arr[0] = 0;
    phi_arr[1] = 1;
    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (phi_arr[i] == 0) {            /* i is prime */
            phi_arr[i] = i - 1;
            primes[pc++] = i;
        }
        for (size_t j = 0; j < pc; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > MAX_LIMIT)
                break;
            if (i % p == 0) {
                phi_arr[v] = phi_arr[i] * p;
                break;
            } else {
                phi_arr[v] = phi_arr[i] * (p - 1);
            }
        }
    }
    free(primes);
    phi_ready = true;
}

/*-------------------------------------------------------------
 *  Cache of the sequence values
 *-------------------------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure cache has at least 'need' slots */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t ncap = (cache_cap == 0) ? 4 : cache_cap;
    while (ncap < need)
        ncap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, ncap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = ncap;
}

/*-------------------------------------------------------------
 *  Main sequence function
 *-------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the OEIS offset */
        return 0;

    init_phi();

    static idx_t next_m = 1;   /* the next m to test */

    while (cache_len < (size_t)n) {
        if (next_m > MAX_LIMIT) abort();  /* safety; should never happen */

        idx_t phi_m = phi_arr[next_m];
        seq_t candidate = (seq_t)next_m * (seq_t)phi_m + 1;

        if (is_prime_big(candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = candidate;
        }
        ++next_m;
    }
    return cache[n - 1];   /* a(1) is the first stored value */
}
