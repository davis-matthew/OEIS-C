/*
 * A040119  Primes p such that x^4 = 10 has a solution mod p.
 * https://oeis.org/A040119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 985 (2026-11-03)
 * Helpers:   isqrt, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements A040119: primes p such that x^4 = 10 has a solution mod p.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt.h"
#include "../utils/powmod.h"
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

static seq_t  *seq        = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;

static uint64_t *primes    = NULL;
static size_t   primes_len = 0;
static size_t   primes_cap = 0;

static uint64_t next_candidate = 3;   /* next odd number to test for primality */
static bool initialized = false;

/* Return true iff prime p satisfies x^4 = 10 (mod p). */
static bool good_prime(uint64_t p) {
    if (p == 2 || p == 5) return true;   /* 10 ≡ 0 (mod p) */
    uint64_t g;
    uint64_t pm1 = p - 1;
    if ((pm1 & 3) == 0)               /* p-1 divisible by 4 */
        g = 4;
    else if ((pm1 & 1) == 0)          /* p-1 divisible by 2 only */
        g = 2;
    else
        g = 1;
    uint64_t exp = pm1 / g;
    seq_t r = powmod((seq_t)10, exp, (seq_t)p);
    return r == 1;
}

/* Add p to the global prime list and, if it is a “good” prime, to the
   sequence of terms. */
static void add_prime(uint64_t p) {
    /* enlarge primes array if necessary */
    ensure_capacity(primes_len + 1, &primes_cap, OEISC_VPP(&primes), sizeof(uint64_t));
    primes[primes_len++] = p;

    if (good_prime(p)) {
        ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq), sizeof(seq_t));
        seq[seq_len++] = (seq_t)p;
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        /* initialise storage */
        ensure_capacity(1, &primes_cap, OEISC_VPP(&primes), sizeof(uint64_t));
        primes_len = 0;
        add_prime(2);          /* first prime */
        next_candidate = 3;
        initialized = true;
    }

    while (seq_len < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;   /* only odd candidates */

        /* trial division by known primes up to sqrt(cand) */
        bool isprime = true;
        uint64_t lim = isqrt(cand);
        for (size_t i = 0; i < primes_len && primes[i] <= lim; ++i) {
            if (cand % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            add_prime(cand);
        }
    }
    return seq[n-1];
}
