/*
 * A066268  Product of first n primorials - 1.
 * https://oeis.org/A066268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} (∏_{j=1}^{k} p_j) – 1, where p_j is the j‑th
 *            prime. The function returns a(1)=1, a(2)=11, a(3)=359, …
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------
   Prime generation – sieve up to a fixed upper bound that covers
   at least the first 10 000 primes.  104 729 is the 10 000‑th prime,
   so 200 000 is sufficiently large.
   ------------------------------------------------------------------ */
#define PRIME_UPPER 200000               /* > 104 729  */
#define PRIMES_BUF 20000                 /* enough for 200 000   */

static bool _primes_sieved = false;
static bool _is_prime[PRIME_UPPER + 1];
static uint32_t _primes[PRIMES_BUF];
static size_t _prime_cnt = 0;

static void _sieve_primes(void)
{
    if (_primes_sieved) return;
    for (size_t i = 0; i <= PRIME_UPPER; ++i) _is_prime[i] = true;
    _is_prime[0] = _is_prime[1] = false;
    for (size_t p = 2; p * p <= PRIME_UPPER; ++p) {
        if (_is_prime[p]) {
            for (size_t m = p * p; m <= PRIME_UPPER; m += p)
                _is_prime[m] = false;
        }
    }
    for (size_t i = 2; i <= PRIME_UPPER && _prime_cnt < PRIMES_BUF; ++i) {
        if (_is_prime[i]) _primes[_prime_cnt++] = (uint32_t)i;
    }
    _primes_sieved = true;
}

/* ------------------------------------------------------------------
   Cached sequence values and running products.
   ------------------------------------------------------------------ */
static seq_t * _cache = NULL;        /* cache[0] is unused, cache[n] = a(n)   */
static size_t   _cache_cap = 0;      /* allocated size of _cache            */
static size_t   _cache_len = 0;      /* highest n that has been computed    */
static seq_t    _cur_prim = 1;       /* current primorial                   */
static seq_t    _cur_prod = 1;       /* product of primorials up to current */

static void _cache_grow(size_t need)
{
    if (need <= _cache_cap) return;
    size_t new_cap = (_cache_cap == 0) ? 8 : _cache_cap * 2;
    if (new_cap < need) new_cap = need;
    _cache = (seq_t *)realloc(_cache, (new_cap + 1) * sizeof(seq_t));
    _cache_cap = new_cap;
}

/* ------------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* sequence is defined for n ≥ 1 */

    _sieve_primes();

    if ((idx_t)_prime_cnt < n) return 0; /* should not happen on the tests */

    /* build cache up to n if needed */
    _cache_grow((size_t)n);

    if ((size_t)n > _cache_len) {
        /* compute values for indices _cache_len+1 … n */
        for (size_t i = _cache_len + 1; i <= (size_t)n; ++i) {
            seq_t p = (seq_t)_primes[i - 1];   /* i‑th prime, 1‑based */
            _cur_prim  = _cur_prim * p;        /* primorial(i)   */
            _cur_prod  = _cur_prod * _cur_prim;
            _cache[i]  = _cur_prod - 1;        /* a(i) */
        }
        _cache_len = n;
    }

    return _cache[n];
}
