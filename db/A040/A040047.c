/*
 * A040047  Primes p such that x^3 = 6 has no solution mod p.
 * https://oeis.org/A040047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 913 (2026-11-03)
 * Helpers:   isqrt, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes p such that the congruence x^3 ≡ 6 (mod p) has no solution.
   For a prime p ≡ 2 (mod 3) every non‑zero element is a cube,
   so such p are excluded.  For p ≡ 1 (mod 3) 6 is a cube iff
   6^((p-1)/3) ≡ 1 (mod p).  We collect the primes where this does not hold. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helpers from the utility library */
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

/* Static cache for primes and the sequence */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;

static seq_t *good = NULL;   /* primes where 6 is not a cubic residue */
static size_t good_cnt = 0, good_cap = 0;

/* Incrementally generate primes and collect good primes until we have at least n_needed. */
static void generate_more_good_primes(idx_t n_needed)
{
    static idx_t cand = 2;               /* next candidate to test for primality */

    while (good_cnt < n_needed) {
        bool prime_flag;
        if (cand == 2 || cand == 3) {
            prime_flag = true;
        } else {
            prime_flag = true;
            idx_t sqrt_cand = isqrt((idx_t)cand);
            for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_cand; ++i) {
                if (cand % primes[i] == 0) {
                    prime_flag = false;
                    break;
                }
            }
        }

        if (prime_flag) {
            /* store prime */
            ensure_capacity(primes_cnt + 1, &primes_cap, OEISC_VPP(&primes), sizeof(uint64_t));
            primes[primes_cnt++] = cand;

            /* test the cubic residue condition only for primes ≡ 1 (mod 3) */
            if (cand % 3 == 1) {
                seq_t res = powmod((seq_t)6, (idx_t)((cand - 1) / 3), (seq_t)cand);
                if (res != (seq_t)1) {
                    ensure_capacity(good_cnt + 1, &good_cap, OEISC_VPP(&good), sizeof(seq_t));
                    good[good_cnt++] = (seq_t)cand;
                }
            }
        }

        /* advance to next candidate */
        if (cand == 2)
            cand = 3;
        else
            cand += 2;   /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n == 0, but safe guard */
    generate_more_good_primes(n);
    return good[n - 1];
}
