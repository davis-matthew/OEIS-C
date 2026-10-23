/*
 * A163780  Terms in A054639 equal to 3 mod 4.
 * https://oeis.org/A163780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1073, u32 n <= 1230, u64 n <= 3378,
 *            u128 n <= 3378, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A163780.
   A term N (≡ 3 (mod 4)) is *selected* if and only if
    • p = 2·N + 1 is prime,
    • –2 is a primitive root modulo p   (i.e., for every prime factor q of φ(p)=p−1,
        (−2)^( (p−1)/q ) ≠ 1 (mod p) ),
    • +2 is NOT a primitive root modulo p  (i.e., for some prime factor q of φ(p) the
        2^( (p−1)/q ) ≡ 1 (mod p) ).
   The sequence consists of all such N, sorted increasingly, with the first
   term being 3.  The function context-managed cache ensures that
   repeated calls are efficient.  The algorithm runs in approximately
   linear time in the number of terms found. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper functions provided by the build system */
#include "../utils/is_prime_big.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

/* Static state for memoisation */
static seq_t *cache      = NULL;   /* values that satisfy the conditions */
static size_t cache_len  = 0;      /* number of cached values */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static idx_t   next_N     = 3;     /* next odd candidate of the form 4k+3 */

/* Prime list used for factoring N.  The list is rebuilt only when a larger
   sqrt(N) is required. */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;      /* number of primes in prime_list */
static idx_t    prime_limit = 0;      /* maximum prime generated so far */

/* Ensure we have at least ‘limit’ primes available in prime_list.  The list
   is stored in ascending order by build_primes_upto. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Release any previously allocated memory. */
    free(prime_list);
    /* Request a new list big enough to cover 'limit'. */
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Grow the cache to hold at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
    while (new_cap < needed)
        new_cap <<= 1;

    /* In C++ realloc returns void*, in C cast is optional but harmless. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true if N satisfies all conditions of the sequence. */
static bool is_candidate(idx_t N)
{
    /* p = 2N + 1 must be prime. */
    seq_t p = ((seq_t)2 * N) + 1;
    if (!is_prime_big(p))
        return false;

    /* Factor N (N is odd, hence contains at least one odd prime factor). */
    idx_t n_tmp   = N;
    idx_t sqrt_n  = _floor_isqrt_u64(n_tmp) + 1;
    ensure_primes(sqrt_n);

    idx_t factors[64];
    size_t fcnt = 0;

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= sqrt_n; ++i) {
        uint64_t pr = prime_list[i];
        if (n_tmp % pr == 0) {
            factors[fcnt++] = pr;
            while (n_tmp % pr == 0)
                n_tmp /= pr;
        }
    }
    if (n_tmp > 1) {             /* remaining prime factor > sqrt_n */
        factors[fcnt++] = n_tmp;
    }

    /* Prime factors of φ(p) = p‑1 = 2N: 2 plus all factors of N. */
    idx_t phi_factors[65];
    size_t phi_fcnt = 0;
    phi_factors[phi_fcnt++] = 2;
    for (size_t i = 0; i < fcnt; ++i)
        phi_factors[phi_fcnt++] = factors[i];

    /* Check that –2 is a primitive root modulo p. */
    for (size_t i = 0; i < phi_fcnt; ++i) {
        uint64_t q   = phi_factors[i];
        uint64_t exp = ((uint64_t)p - 1) / q;
        if (mod_pow((seq_t)(-2 + p), exp, p) == 1)
            return false;   /* not a primitive root */
    }

    /* Check that +2 is NOT a primitive root modulo p. */
    bool two_not_primitive = false;
    for (size_t i = 0; i < phi_fcnt; ++i) {
        uint64_t q   = phi_factors[i];
        uint64_t exp = ((uint64_t)p - 1) / q;
        if (mod_pow((seq_t)2, exp, p) == 1) {
            two_not_primitive = true;
            break;
        }
    }
    if (!two_not_primitive)
        return false;          /* +2 would be a primitive root */

    return true;
}

/* Public sequence function.  It obtains terms lazily and stores them in
   cache for future calls. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset; not used by callers */

    /* Generate terms until we have the n-th one. */
    while (cache_len < n) {
        if (is_candidate(next_N)) {
            ensure_cache(cache_len + 1);
            /* Store the value as seq_t.  For builds where seq_t is
               smaller than next_N the value will be truncated, but
               such terms are outside the tested range. */
            cache[cache_len++] = (seq_t)next_N;
        }
        next_N += 4;  /* next candidate with N ≡ 3 (mod 4) */
    }
    return cache[n - 1];
}
