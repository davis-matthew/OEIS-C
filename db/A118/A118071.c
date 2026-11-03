/*
 * A118071  Primes which are the sum of a twin prime pair + 1.
 * https://oeis.org/A118071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 111, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9756 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A118071
 * a(n) = 2*p + 3, where p and p+2 are consecutive twin primes.
 * The sequence starts: 13,37,61,277,...
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static uint64_t *prime_list = NULL;   /* primes up to current limit          */
static size_t prime_cnt    = 0;       /* number of primes in prime_list      */
static uint64_t limit      = 0;       /* current sieve limit                 */
static size_t twin_start   = 0;       /* index of lower twin prime to test   */

static seq_t *terms       = NULL;    /* cached sequence values              */
static size_t terms_cnt   = 0;
static size_t terms_cap   = 0;

/* extend the prime sieve so that it contains all primes ≤ new_limit */
static void extend_primes(uint64_t new_limit)
{
    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(new_limit, &new_list, &new_cnt);
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    limit      = new_limit;
}

/* make sure that the sequence contains at least n terms   */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;

    /* Initial sieve */
    if (limit == 0)
        extend_primes(200000ULL);

    while (terms_cnt < (size_t)n)
    {
        /* Make sure we can look at the next candidate twin prime pair */
        while (twin_start + 1 >= prime_cnt)
            extend_primes(limit * 2ULL);

        /* Check if prime_list[twin_start] and prime_list[twin_start+1]
         * form a twin prime pair */
        if (prime_list[twin_start + 1] == prime_list[twin_start] + 2ULL)
        {
            /* Candidate = lower twin prime * 2 + 3 */
            seq_t candidate = (seq_t)prime_list[twin_start];
            candidate = candidate * (seq_t)2 + (seq_t)3;
            if (is_prime_big(candidate))
            {
                ensure_capacity(terms_cnt + 1, &terms_cap,
                               OEISC_VPP(&terms), sizeof(seq_t));
                terms[terms_cnt++] = candidate;
                /* After adding, we may have enough terms -> exit if desired */
                /* but the outer loop will check again */
            }
        }
        /* Move to next prime for the next trial */
        twin_start++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; indices <1 are undefined */
        return (seq_t)0;
    ensure_terms(n);
    return terms[n - 1];
}
