/*
 * A167187  Primes obtained from other primes by prefixing a 7.
 * https://oeis.org/A167187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 776 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A167187: Primes obtained by prefixing a 7 to a prime.
 * a(n) is the n‑th prime of the form 7·10^d(p)+p,
 * where p is a prime and d(p) is the number of decimal digits of p.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Utility function headers */
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

/* ----- static storage for memoisation ----- */
static seq_t *terms      = NULL;  /* array of sequence values   */
static size_t terms_cap  = 0;     /* allocated capacity         */
static size_t terms_cnt  = 0;     /* number of filled terms     */

static uint64_t *primes      = NULL;  /* array of primes up to limit   */
static size_t primes_cnt     = 0;     /* number of primes in array     */
static idx_t   prime_limit   = 0;     /* current upper bound for primes*/
static size_t prime_idx_checked = 0;  /* index of next prime to test    */

/* Expand the prime list up to (and including) new_limit. */
static void extend_primes(idx_t new_limit)
{
    if (new_limit <= prime_limit)
        return;

    /* free old prime array if it exists */
    if (primes != NULL)
        free(primes);

    build_primes_upto(new_limit, &primes, &primes_cnt);

    prime_limit = new_limit;
}

/* Ensure we can store at least need elements in terms array. */
static void ensure_terms_capacity(size_t need)
{
    ensure_capacity(need, &terms_cap, OEISC_VPP(&terms), sizeof(seq_t));
}

/* Generate terms up to a(n).  (All indices are 1‑based.) */
static void compute_terms_up_to(idx_t n)
{
    /* Start with a moderate upper bound for primes */
    if (prime_limit == 0)
        extend_primes(200000);

    while (terms_cnt < n) {
        /* Produce more primes if we have reached the end of the list. */
        while (prime_idx_checked >= primes_cnt) {
            extend_primes(prime_limit * 2 + 1);   /* double the limit */
            prime_idx_checked = 0;               /* restart scanning  */
        }

        /* Test the next prime p. */
        uint64_t p = primes[prime_idx_checked++];
        idx_t d  = num_digits_idx((idx_t)p);          /* number of decimal digits */
        seq_t pow10d  = pow10((idx_t)d);      /* 10^d as seq_t          */
        seq_t candidate = (seq_t)7 * pow10d + (seq_t)p;  /* 7‖p */

        if (is_prime_big(candidate)) {
            ensure_terms_capacity(terms_cnt + 1);
            terms[terms_cnt++] = candidate;   /* store the prime candidate */
        }
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined, but safe for 0 */
        return (seq_t)0;

    compute_terms_up_to(n);
    return terms[n - 1];
}
