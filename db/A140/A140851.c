/*
 * A140851  Primes of the form 210k + 53.
 * https://oeis.org/A140851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* OEIS sequence A140851:
   primes of the form 210*k + 53, enumerated for k = 0,1,2,… */

/* dynamically grow an array */
#ifdef __cplusplus
template <class T>
static T *grow_array(T *ptr, size_t *cap, size_t need, size_t elem_size) {
    if (*cap >= need) return ptr;
    size_t new_cap = (*cap == 0) ? 64 : *cap * 2;
    while (new_cap < need) new_cap *= 2;
    T *np = mem_resize(T, ptr, (new_cap * elem_size) / sizeof(T));
    if (!np) abort();
    *cap = new_cap;
    return np;
}
#else
static void *grow_array(void *ptr, size_t *cap, size_t need, size_t elem_size) {
    if (*cap >= need) return ptr;
    size_t new_cap = (*cap == 0) ? 64 : *cap * 2;
    while (new_cap < need) new_cap *= 2;
    void *np = realloc(ptr, new_cap * elem_size);
    if (!np) abort();
    *cap = new_cap;
    return np;
}
#endif

/* static cache of primes of the required form */
static seq_t *prime_cache = NULL;     /* array of found primes          */
static size_t prime_cap   = 0;        /* allocated size of prime_cache  */
static size_t prime_cnt   = 0;        /* number of primes stored        */
static idx_t   next_k     = 0;        /* next k to test (candidate = 210*k+53) */

/* generate primes until we have at least target primes stored */
static void generate_to(idx_t target) {
    while (prime_cnt < target) {
        seq_t cand = (seq_t)(210 * next_k + 53);   /* 210*k + 53 */
        if (is_prime_big(cand)) {
            prime_cache = (seq_t *)grow_array(prime_cache, &prime_cap,
                                              prime_cnt + 1, sizeof(seq_t));
            prime_cache[prime_cnt++] = cand;
        }
        ++next_k;
    }
}

/* OEIS sequence A140851 */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, return 0 as a safe value */
    generate_to(n);
    return prime_cache[n - 1];
}
