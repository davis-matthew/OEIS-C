/*
 * A235144  Primes whose base-10 representation also represents a prime in
 *          base 19.
 * https://oeis.org/A235144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 321, u32 n <= 3276, u64 n <= 3276,
 *            u128 n <= 3276, big any n
 * Validated: standard, pass, 76 terms through n = 3208 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS sequence A235144:
   a(n) = the n-th prime p such that the number obtained by interpreting
   the decimal digits of p as a base‑19 number is also prime.
   The sequence is generated on demand and memoised. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Static state used for memoisation */
static seq_t *term_cache = NULL;          /* array of found terms         */
static size_t term_cap   = 0;             /* capacity of term_cache       */
static size_t term_len   = 0;             /* how many terms are stored    */

static uint64_t *prime_list   = NULL;     /* list of primes up to limit   */
static size_t    primes_cnt   = 0;        /* number of primes in list     */
static uint64_t  primes_limit = 0;        /* current sieve upper bound    */
static size_t    primes_done  = 0;        /* number of primes already examined */

/* Helper: extend term_cache to hold at least 'need' entries */
static void grow_term_cache(size_t need)
{
    if (need <= term_cap) return;
    ensure_capacity(need, &term_cap, OEISC_VPP(&term_cache), sizeof(seq_t));
}

/* Generate terms up to index 'target' (1‑based).  */
static void generate_terms(idx_t target)
{
    if ((size_t)target <= term_len) return;

    /* initialise prime list if necessary */
    if (prime_list == NULL) {
        primes_limit = 1000000;                     /* start with 1M */
        build_primes_upto(primes_limit, &prime_list, &primes_cnt);
    }

    while (term_len < (size_t)target) {
        /* if we have exhausted the current prime list, extend it */
        if (primes_done >= primes_cnt) {
            primes_limit *= 2;                     /* double the limit */
            free(prime_list);
            build_primes_upto(primes_limit, &prime_list, &primes_cnt);
        }

        /* examine next prime */
        uint64_t p = prime_list[primes_done++];
        /* compute q = interpret decimal digits of p in base 19 */
        uint64_t temp = p;
        uint8_t digits[25];
        int len = 0;
        if (temp == 0) {              /* prime cannot be zero, but guard anyway */
            digits[len++] = 0;
        } else {
            while (temp) {
                digits[len++] = (uint8_t)(temp % 10);
                temp /= 10;
            }
        }
        uint64_t q = 0;
        for (int i = len - 1; i >= 0; --i) {
            q = q * 19 + digits[i];
        }

        /* test primality of q */
        if (isPrimeGeneric((seq_t)q)) {
            grow_term_cache(term_len + 1);
            term_cache[term_len++] = (seq_t)p;   /* store the prime itself */
        }
    }
}

/* The required public function */
extern seq_t a(idx_t n)
{
    generate_terms(n);            /* ensure term n is computed */
    return term_cache[(size_t)n - 1];
}
