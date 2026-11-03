/*
 * A126143  Primes whose sum of the decimal digits of their cubes is also a
 *          prime.
 * https://oeis.org/A126143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n‑th prime p such that the sum of the decimal digits
 *            of p³ is also a prime
 * Range:     u8 none, u16 n <= 4, u32 n <= 100, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 69 terms through n = 863 (2026-11-03)
 * Helpers:   ipow, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/is_prime_seq.h"
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

/* ---------- static state ---------- */
static seq_t *terms      = NULL;   /* array of found terms */
static size_t terms_len  = 0;      /* number of stored terms    */
static size_t terms_cap  = 0;      /* allocated capacity       */
static idx_t  last_checked = 0;    /* last integer examined    */

/* Expand the terms array so it can store at least 'need' elements. */
static void ensure_terms_cap(void)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        ensure_capacity(new_cap, &terms_cap, OEISC_VPP(&terms), sizeof(seq_t));
    }
}

/* Compute the sum of decimal digits of a seq_t value.  The result
   fits into a native integer but we keep it in seq_t for uniformity. */
static seq_t digit_sum(seq_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Return the n‑th term of sequence A126143.  a(1) is the first
   prime satisfying the condition.  For n = 0 we return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but handled gracefully */

    /* Generate terms until at least n are found. */
    while (terms_len < n) {
        /* Find the next integer > last_checked that may be prime. */
        idx_t cand_i;
        do {
            ++last_checked;
            cand_i = last_checked;
        } while (cand_i > 2 && (cand_i & 1) == 0); /* skip evens after 2 */

        seq_t cand = (seq_t)cand_i;

        /* Test whether cand is prime.  is_prime_big works for any seq_t. */
        if (!is_prime_big(cand))
            continue;  /* not prime – keep searching */

        /* Sum of decimal digits of the cube. */
        seq_t cube = ipow(cand, 3);
        seq_t s    = digit_sum(cube);

        /* If that sum is also prime -> store the prime. */
        if (is_prime_seq(s)) {
            ensure_terms_cap();
            terms[terms_len++] = cand;
        }
    }

    return terms[n - 1];
}
