/*
 * A145650  Linking prime for the first and second member of maximal chains of
 *          primes that have at least three members.
 * https://oeis.org/A145650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the linking prime for the first two members of each *
 *            maximal chain of consecutive primes of length at least 3,
 *            ordered by the * chain starting prime. The function searches
 *            primes incrementally, * extends each found chain as far as
 *            possible, records the linking prime * of the start, and s...
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static uint64_t *primes = NULL;   /* 1‑based array of primes */
static size_t   primes_len = 0;   /* number of primes stored       */
static uint64_t sieve_limit = 0;  /* current sieve upper bound     */

/*--- sieve helper -------------------------------------------------------*/
static void recompute_sieve(uint64_t limit)
{
    if (sieve_limit >= limit) return;                 /* enough already */
    char *is_comp = (char *)malloc(limit + 1);
    if (!is_comp) abort();
    memset(is_comp, 0, limit + 1);
    is_comp[0] = is_comp[1] = 1;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    if (primes) {
        free(primes);
        primes = NULL;
    }
    primes_len  = 0;
    size_t cap  = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) {
            if (primes_len == cap) {
                cap = (cap == 0) ? 128 : cap * 2;
                uint64_t *np = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
                if (!np) abort();
                primes = np;
            }
            primes[primes_len++] = i;
        }
    free(is_comp);
    sieve_limit = limit;
}

/*--- ensure enough primes for index -----------------------------------*/
static void ensure_primes_needed(idx_t needed)          /* 1‑based count */
{
    while (primes_len < (size_t)needed) {
        uint64_t new_limit = (sieve_limit == 0) ? 1000 : sieve_limit * 2;
        recompute_sieve(new_limit);
    }
}

/*--- sequence cache ---------------------------------------------------*/
static seq_t *cache   = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t  next_index = 1;      /* next prime index to test (1‑based) */

static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *np = mem_resize(seq_t, cache, cache_cap);
        if (!np) abort();
        cache = np;
    }
    cache[cache_len++] = val;
}

/*--- main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */

    while (cache_len < n) {
        idx_t i = next_index;                 /* candidate start index */

        /* ensure we have primes up to i+1 for the first linking test */
        ensure_primes_needed(i + 1);

        uint64_t p_i   = primes[i - 1];
        uint64_t p_ip  = primes[i];
        seq_t   L0 = (seq_t)i * (seq_t)p_i + (seq_t)(i + 1) * (seq_t)p_ip;

        if (!is_prime_big(L0)) {              /* first link fails */
            ++next_index;
            continue;
        }

        /* extend the chain as far as possible */
        idx_t j = i + 1;
        size_t chain_len = 2;
        while (1) {
            ensure_primes_needed(j + 1);
            uint64_t p_j  = primes[j - 1];
            uint64_t p_jp = primes[j];
            seq_t   Lj = (seq_t)j * (seq_t)p_j + (seq_t)(j + 1) * (seq_t)p_jp;
            if (is_prime_big(Lj)) {
                ++chain_len;
                ++j;
            } else {
                break;
            }
        }

        if (chain_len >= 3) // maximal chain of at least 3 primes
            cache_append(L0);

        /* skip past the whole chain */
        next_index = j + 1;
    }
    return cache[n - 1];
}
