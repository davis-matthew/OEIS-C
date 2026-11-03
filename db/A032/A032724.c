/*
 * A032724  Numbers k such that k prefixed by '8' and followed by '7' is
 *          prime.
 * https://oeis.org/A032724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032724: numbers k such that 8·10^{len(k)+1}+k·10+7 is prime */

/* The sequence is produced by scanning successive k starting from 1,
   testing primality of the 8*10^{len(k)+1}+k*10+7 expression, and
   memoising the qualifying k. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/num_digits_idx.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static void seq_free(seq_t *p) { mem_free(p); }

static seq_t *cache = NULL;
static idx_t cache_cap = 0;    /* allocated size of cache */
static idx_t cache_count = 0;  /* number of stored values */
static idx_t next_k = 1;       /* next k to test */

/* Helper: grow the cache to at least new_cap entries */
static void grow_cache(idx_t new_cap)
{
    seq_t *newarr = seq_alloc(new_cap);
    if (!newarr) {
        /* Allocation failed – abort as the sequence cannot be continued */
        abort();
    }
    for (idx_t i = 0; i < cache_count; ++i)
        newarr[i] = cache[i];
    seq_free(cache);
    cache = newarr;
    cache_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset is 1; value for n==0 is irrelevant */

    /* Return a stored value if already known */
    if (n <= cache_count)
        return cache[n - 1];

    /* Ensure the cache can grow */
    if (cache_count >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
        grow_cache(new_cap);
    }

    /* Generate new terms until we reach the n‑th one */
    while (cache_count < n) {
        /* Construct the candidate number N = 8·10^{len(k)+1} + k·10 + 7 */
        int len_k = num_digits_idx(next_k);
        seq_t pow10_val = pow10((idx_t)(len_k + 1));
        seq_t N = ((seq_t)8 * pow10_val) + (seq_t)next_k * 10 + 7;

        if (is_prime_big(N)) {
            /* Store the qualifying k */
            cache[cache_count++] = (seq_t)next_k;
            if (cache_count >= cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
                grow_cache(new_cap);
            }
        }
        ++next_k;
    }

    return cache[n - 1];
}
