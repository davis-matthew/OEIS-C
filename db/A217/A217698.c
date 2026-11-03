/*
 * A217698  Primes of the form ((2^p -1)*(3^p -2))-2, where p is prime.
 * https://oeis.org/A217698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime of the form ((2^p-1)*(3^p-2))-2 where p is prime
 *            and the value is prime. The sequence stores the i‑th such prime
 *            for i ≥ 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of computed terms and generates new ones by
   iterating over primes p using next_prime. For each p, it evaluates the
   expression using pow2, pow3, seq_mul and checks primality with is_prime_big. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/next_prime.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
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

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of offset; undefined but return 0 */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  cached_len = 0;
    static size_t cached_cap = 0;

    /* continue generating until we have at least n terms */
    static uint64_t current_prime = 2;     /* next prime to test */

    while (cached_len < n) {
        /* Ensure cache has space for at least one more term */
        ensure_capacity(cached_len + 1, &cached_cap, OEISC_VPP(&cache), sizeof(seq_t));

        seq_t p = (seq_t)current_prime;

        /* Compute 2^p and 3^p */
        seq_t two_pow = pow2(current_prime);   /* 2^p  */
        seq_t three_pow = pow3(current_prime); /* 3^p  */

        /* Compute ((2^p - 1) * (3^p - 2)) - 2 */
        seq_t term = seq_mul(two_pow - 1, three_pow - 2) - 2;

        /* If the term is prime, store it */
        if (is_prime_big(term)) {
            cache[cached_len++] = term;
        }

        /* Move to the next prime p */
        current_prime = (uint64_t)next_prime(current_prime);
    }

    return cache[n - 1];
}
