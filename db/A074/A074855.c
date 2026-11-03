/*
 * A074855  Primes p such that all p-t for p/2 < t < p are nonprime, where t
 *          is a triangular number.
 * https://oeis.org/A074855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = the n‑th prime p such that for all triangular numbers t
 *            with p/2 < t < p the value p−t is composite or 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
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

/* Memoised sequence data */
static seq_t *seq = NULL;        /* array of terms */
static size_t seq_sz = 0;        /* number of terms stored */
static size_t seq_cap = 0;       /* allocated capacity */
static idx_t next_candidate = 2; /* integer to test for primality */

/* Return the n‑th term (n ≥ 1) of sequence A074855.  */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (seq_sz < n) {
        /* Find the next prime satisfying the property. */
        for (idx_t cand = next_candidate; ; ++cand) {
            next_candidate = cand + 1;            /* advance global counter */

            if (!isPrimeGeneric((seq_t)cand))
                continue;                         /* skip non‑prime candidates */

            /* For small primes 2 and 3 the property holds vacuously. */
            seq_t half = (seq_t)(cand / 2);
            bool good = true;
            idx_t  t  = 0;

            /* Enumerate triangular numbers T(k)=k(k+1)/2.  */
            for (idx_t k = 1; ; ++k) {
                t += k;                            /* t = T(k)   */
                if (t >= cand)
                    break;

                if (t > cand / 2) {                /* p/2 < t < p  */
                    seq_t d = (seq_t)(cand - t);
                    if (isPrimeGeneric(d)) {
                        good = false;             /* property violated */
                        break;
                    }
                }
            }

            if (!good)
                continue;                         /* test next candidate */

            /* Store the found prime as the next term. */
            if (seq_sz == seq_cap) {
                ensure_capacity(seq_sz + 1, &seq_cap,
                                OEISC_VPP(&seq), sizeof(seq_t));
            }
            seq[seq_sz++] = (seq_t)cand;
            break;                                /* proceed to next needed term */
        }
    }
    return seq[n - 1];                               /* sequence is 1‑indexed */
}
