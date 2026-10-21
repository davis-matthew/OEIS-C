/*
 * A067934  Let rep(k) = (10^k - 1)/9 be the k-th repunit number = 11111..1111
 *          with k 1 digits, then sequence gives values of k such that rep(k)
 *          == 1 (mod k).
 * https://oeis.org/A067934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 6624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A067934
 * a(n) = the n‑th integer k such that rep(k) = (10^k-1)/9 ≡ 1 (mod k)
 * Equivalently 10^k ≡ 10 (mod 9·k).
 * The code tests this congruence directly by modular exponentiation
 * and caches found terms to answer any subsequent call quickly.
 * The implementation works for all supported builds: 8‑/16‑/32‑/64‑bit
 * unsigned types as well as the arbitrary‑precision class used by the
 * C++ build.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- *
 *  Helper: modular exponentiation (64‑bit base, exponent, modulus).
 *  Works because the modulus 9·k stays within 64 bits for all tested
 *  values (k < 2^63 / 9).  A 128‑bit intermediate is used to avoid
 *  overflow.
 * --------------------------------------------------------------------- */
static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base   = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* --------------------------------------------------------------------- *
 *  Cache of the sequence values.  cache[i] holds the (i+1)‑th term.
 *  The cache grows lazily as a() requests more terms.
 * --------------------------------------------------------------------- */
static seq_t *cache  = NULL;      /* pointer to array of seq_t          */
static size_t cache_cap = 0;      /* allocated capacity of cache       */
static idx_t  cache_len = 0;      /* number of terms actually stored   */
static idx_t  last_k    = 0;      /* last integer k that was tested    */

/* --------------------------------------------------------------------- *
 *  Resize cache to at least 'needed' capacity.
 *  Uses malloc/realloc and explicit casts so it compiles as both C
 *  and C++.
 * --------------------------------------------------------------------- */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)      /* allocation failure */
        abort();

    cache      = tmp;
    cache_cap  = new_cap;
}

/* --------------------------------------------------------------------- *
 *  Test whether the integer k satisfies 10^k ≡ 10 (mod 9·k).
 *  k == 1 is always a valid term.
 * --------------------------------------------------------------------- */
static int valid_candidate(idx_t k)
{
    if (k == 1)
        return 1;

    uint64_t mod = 9ULL * (uint64_t)k;
    uint64_t r   = pow_mod_uint64(10ULL, (uint64_t)k, mod);
    return r == (10ULL % mod);
}

/* --------------------------------------------------------------------- *
 *  The required sequence function.
 *  Returns the n‑th term (1‑based).  Calls with n < 1 simply return 0.
 * --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                /* safeguard – out of range

/* --------------------------------------------------------------------- *
 *  Generate terms until the cache holds at least n entries.
 * --------------------------------------------------------------------- */
    while (cache_len < n) {
        ++last_k;
        if (valid_candidate(last_k)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
