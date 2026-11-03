/*
 * A086402  Nontrivial numbers which are prime and yield another prime when
 *          their digits are sorted in descending order.
 * https://oeis.org/A086402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A086402: primes whose digits are not already
   in descending order and whose descending‑digit permutation is also
   prime. The algorithm builds the list of such primes on demand,
   caching previously found terms.  */
       
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/get_digits.h"
#include "../utils/num_digits.h"
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

static seq_t *terms      = NULL;   /* cached sequence values          */
static size_t terms_len  = 0;       /* number of cached terms          */
static size_t terms_cap  = 0;       /* allocated capacity for terms    */

static idx_t *prime_list = NULL;   /* list of primes up to sieve limit*/
static size_t prime_list_len  = 0;  /* number of primes in prime_list  */
static size_t prime_proc_idx  = 0;  /* next prime to test              */
static size_t prime_list_cap  = 0;  /* capacity of prime_list          */

static idx_t sieve_limit = 0;       /* current upper bound of the sieve */

/* Extend the sieve up to at least new_limit and rebuild prime_list. */
static void extend_sieve(idx_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    static char *is_comp = NULL;   /* 0 = prime candidate, 1 = composite */
    static size_t comp_cap = 0;

    if ((size_t)new_limit + 1 > comp_cap) {
        free(is_comp);
        is_comp = (char *)malloc((size_t)new_limit + 1);
        comp_cap = (size_t)new_limit + 1;
    }
    /* clear marks */
    for (size_t i = 0; i <= (size_t)new_limit; ++i) is_comp[i] = 0;

    /* classic Eratosthenes sieve */
    for (size_t i = 2; i * i <= (size_t)new_limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= (size_t)new_limit; j += i)
                is_comp[j] = 1;
        }
    }
    /* Count primes to allocate */
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)new_limit; ++i)
        if (!is_comp[i]) count++;

    /* Reallocate prime list if needed */
    ensure_capacity(count, &prime_list_cap, OEISC_VPP(&prime_list),
                    sizeof(idx_t));
    prime_list_len = 0;
    for (size_t i = 2; i <= (size_t)new_limit; ++i)
        if (!is_comp[i]) prime_list[prime_list_len++] = (idx_t)i;

    sieve_limit = new_limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, but safe */

    /* Return from cache if already computed */
    if (n <= terms_len) return terms[n - 1];

    /* Initialise sieve on first call */
    if (sieve_limit == 0) extend_sieve(1000);

    while (terms_len < n) {
        /* Ensure we have primes to process */
        if (prime_proc_idx >= prime_list_len) {
            /* Double the sieve limit until we can extend further */
            idx_t new_limit = sieve_limit * 2;
            extend_sieve(new_limit);
            continue;
        }

        /* Candidate prime */
        idx_t p_val = prime_list[prime_proc_idx++];
        seq_t p = (seq_t)p_val;

        /* Get digits of p */
        size_t len = (size_t)num_digits(p);
        uint8_t *digits = (uint8_t *)malloc(len);
        if (!digits) exit(EXIT_FAILURE);
        size_t used = len;
        get_digits(p, digits, &used);

        /* Sort digits in descending order (simple insertion sort) */
        for (size_t i = 1; i < used; ++i) {
            uint8_t key = digits[i];
            size_t j = i;
            while (j > 0 && digits[j - 1] < key) {
                digits[j] = digits[j - 1];
                --j;
            }
            digits[j] = key;
        }

        /* Reconstruct q from sorted digits */
        seq_t q = (seq_t)0;
        for (size_t i = 0; i < used; ++i)
            q = q * (seq_t)10 + (seq_t)digits[i];

        free(digits);

        /* Accept p if it is non‑trivial and yields a prime when sorted */
        if (p != q && is_prime_big(q)) {
            ensure_capacity(terms_len + 1, &terms_cap,
                            OEISC_VPP(&terms), sizeof(seq_t));
            terms[terms_len++] = p;
        }
    }
    return terms[n - 1];
}
