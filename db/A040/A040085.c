/*
 * A040085  Primes p such that x^3 = 20 has a solution mod p.
 * https://oeis.org/A040085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 932 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th prime p satisfying that x^3 = 20 has a solution mod p.
   The test uses the fact that for odd prime p:
   - If p ≡ 2 (mod 3) then every element is a cube.
   - If p ≡ 1 (mod 3) then 20 is a cube iff 20^((p‑1)/3) ≡ 1 (mod p). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

/* Integer square‑root for uint64_t using binary search.  */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    if (x < 2) return x;
    uint64_t lo = 1, hi = x >> 1, mid, root = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            root = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return root;
}

/* Returns true iff prime p satisfies the cubic residue condition. */
static bool prime_satisfies(uint64_t p)
{
    if (p == 2 || p == 3) return true;              /* trivial small primes */
    if (p % 3 == 2) return true;                    /* all residues are cubes */
    /* p % 3 == 1 : check 20^((p-1)/3) ≡ 1 (mod p) */
    uint64_t exp = (p - 1) / 3;
    seq_t r = powmod((seq_t)20, exp, (seq_t)p);
    return r == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety: offset is 1 */

    /* Static state to allow non‑monotonic calls. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static uint64_t *primes = NULL;
    static size_t primes_size = 0, primes_cap = 0;
    static uint64_t next_candidate = 2;  /* next integer to test for primality */

    while (cache_size < n) {
        uint64_t cand = next_candidate;
        /* Test cand for primality using the list of known primes. */
        bool is_prime = true;
        if (cand >= 2) {
            uint64_t limit = isqrt_uint64(cand);
            for (size_t i = 0; i < primes_size && primes[i] <= limit; ++i) {
                if (cand % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
        } else {
            is_prime = false;
        }

        if (is_prime) {
            /* Store the new prime. */
            ensure_capacity(primes_size + 1, &primes_cap,
                            OEISC_VPP(&primes), sizeof(uint64_t));
            primes[primes_size++] = cand;

            /* Check the cubic residue condition. */
            if (prime_satisfies(cand)) {
                ensure_capacity(cache_size + 1, &cache_cap,
                                OEISC_VPP(&cache), sizeof(seq_t));
                cache[cache_size++] = (seq_t)cand;
            }
        }
        ++next_candidate;     /* advance for future calls */
    }

    return cache[n - 1];
}
