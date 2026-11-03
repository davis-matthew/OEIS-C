/*
 * A141793  Nonprime positive integers of the form k/(prime(k+2)-prime(k+1))
 *          in order of increasing k.
 * https://oeis.org/A141793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = k / (prime(k+2) - prime(k+1)) for the n‑th k such that k
 *            is divisible by the denominator and the quotient is a nonprime
 *            positive integer. (The sequence is generated in increasing order
 *            of the index k.)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* static state shared by all calls */
static seq_t *term_cache  = NULL;   /* cache of computed terms          */
static size_t term_len   = 0;       /* number of terms stored           */
static size_t term_cap   = 0;       /* allocated cache capacity         */

static uint64_t *primes  = NULL;    /* array of primes (prime(1)=2)     */
static size_t    primes_cnt = 0;    /* number of primes in the array    */

static idx_t next_k = 1;           /* next index k to examine          */
static idx_t prime_limit = 20000000; /* initial sieve limit (value)     */

/* initialize prime array once */
static void init_primes(void)
{
    if (primes != NULL)
        return;

    build_primes_upto(prime_limit, &primes, &primes_cnt);

    /* In the unlikely event the limit was insufficient for the
       requested n, we would have to rebuild with a larger limit
       (omitted for brevity).  For n up to ~10000 the chosen limit
       is more than enough. */
}

/* ensure the cache can hold at least `need` elements */
static void ensure_term_cache(size_t need)
{
    if (term_cap >= need)
        return;
    ensure_capacity(need, &term_cap, OEISC_VPP(&term_cache), sizeof(seq_t));
}

/* returns the n-th term (n >= 1), with caching and lazy prime generation */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined, safe return */

    /* fast path if already computed */
    if (term_len >= (size_t)n)
        return term_cache[(size_t)n - 1];

    init_primes();                      /* ensure prime list is ready */

    while (term_len < (size_t)n) {
        /* guard against running out of primes (unlikely for n <= 10000) */
        if ((size_t)(next_k + 1) >= primes_cnt) {
            /* enlarge sieve limit and rebuild */
            prime_limit *= 2;
            free(primes);
            build_primes_upto(prime_limit, &primes, &primes_cnt);
        }

        idx_t diff = primes[next_k + 1] - primes[next_k];
        if (diff != 0 && next_k % diff == 0) {
            seq_t val = (seq_t)(next_k / diff);
            if (!is_prime_big(val)) {           /* w.r.t. seq_t type   */
                ensure_term_cache(term_len + 1);
                term_cache[term_len++] = val;
            }
        }
        ++next_k;
    }
    return term_cache[(size_t)n - 1];
}
