/*
 * A081869  a(1)=2; for n>1, a(n)=2*a(n-1)-1 if that number is composite,
 *          a(n)=a(n-1)+1 otherwise.
 * https://oeis.org/A081869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; for n>1, a(n)=2*a(n-1)-1 if that number is composite,
   a(n)=a(n-1)+1 otherwise. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

static seq_t *cache = NULL;      /* array holding a(1) .. a(k) */
static size_t cache_cap = 0;     /* allocated capacity (number of slots) */
static size_t cache_len = 0;     /* number of computed terms */

/* Ensure that the cache has at least "min" slots. */
static void ensure_capacity(size_t min)
{
    if (cache_cap >= min) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < min) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp && new_cap > 0) abort();      /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for n=0, but guard just in case */
        return (seq_t)0;

    /* Expand cache so that it can store up to index n (1‑based) */
    ensure_capacity((size_t)n);

    /* Cache is empty – initialise the very first term */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;     /* a(1) = 2 */
        cache_len = 1;
    }

    /* If the requested term is already computed, return it */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Compute missing terms, starting from where we left off */
    seq_t prev = cache[cache_len - 1];
    for (idx_t i = cache_len; i < n; ++i) {     /* i corresponds to index i+1 */
        seq_t cand = prev * ((seq_t)2) - (seq_t)1;   /* 2*a(i)-1 */
        seq_t next;
        /* If cand is composite (i.e. not prime), use it;
           otherwise (prime) use prev+1 */
        if (is_prime_big(cand) || cand == (seq_t)1)
            next = prev + (seq_t)1;   /* a(i+1) = a(i)+1 */
        else
            next = cand;             /* a(i+1) = 2*a(i)-1 */

        cache[i] = next;   /* store at zero‑based index i */
        prev = next;
    }

    cache_len = (size_t)n;     /* now we have terms up to n */
    return cache[n - 1];
}
