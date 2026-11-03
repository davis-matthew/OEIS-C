/*
 * A176164  Primes p such that (p-2)/7 is not a prime number.
 * https://oeis.org/A176164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 985 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that (p-2)/7 is not a prime number.
   The implementation generates successive primes and keeps those
   that do not satisfy the condition.  It uses the provided
   is_prime_big() function for primality testing and a simple
   dynamic array to memoize results.  Previous terms are cached so
   that non‑increasing calls to a(idx_t) work efficiently. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* Ensure a dynamic array has capacity for at least *cnt elements. */
#ifdef __cplusplus
template <class T>
static void ensure_cap(size_t *cnt, size_t *cap, T **arr, size_t elem_size)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        T *p = mem_resize(T, *arr, (new_cap * elem_size) / sizeof(T));
        if (!p) abort();                  /* fatal on allocation failure */
        *arr = p;
        *cap = new_cap;
    }
}
#else
static void ensure_cap(size_t *cnt, size_t *cap, void **arr, size_t elem_size)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        void *p = realloc(*arr, new_cap * elem_size);
        if (!p) abort();                  /* fatal on allocation failure */
        *arr = p;
        *cap = new_cap;
    }
}
#endif

/* Static state for incremental prime generation and sequence storage. */
static seq_t *seq_arr   = NULL;
static idx_t  seq_cnt   = 0;
static idx_t  seq_cap   = 0;
static seq_t  next_candidate = 2;             /* next number to test */

/* Return the n‑th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset safety */

    while (seq_cnt < (size_t)n) {
        /* Test candidate for primality. */
        if (is_prime_big(next_candidate)) {
            bool accept = true;
            /* Only primes congruent to 2 mod 7 can be removed. */
            if ((next_candidate % (seq_t)7) == (seq_t)2) {
                seq_t k = (next_candidate - (seq_t)2) / (seq_t)7;
                if (is_prime_big(k))
                    accept = false;
            }
            if (accept) {
                ensure_cap(&seq_cnt, &seq_cap,
                           OEISC_VPP(&seq_arr), sizeof(seq_t));
                seq_arr[seq_cnt++] = next_candidate;
            }
        }
        /* Advance to the next integer.  Overflow is unacceptable
           for practical n <= 10000, but we avoid wrapping. */
        next_candidate = next_candidate + (seq_t)1;
    }
    return seq_arr[(size_t)n - 1];
}
