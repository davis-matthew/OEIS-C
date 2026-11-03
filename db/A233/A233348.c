/*
 * A233348  Numbers n such that 3*n+2 and 3*n-2 are both prime for n multiple
 *          of 5 (A008587).
 * https://oeis.org/A233348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233348
   a(n) = the n-th integer of the form 5*m (m ≥ 1) such that
          3*(5*m) – 2 and 3*(5*m) + 2 are both prime. */

/* External types */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static void seq_free(seq_t *p) { mem_free(p); }

static seq_t *cache      = NULL;   /* array of found terms      */
static size_t cache_size = 0;      /* number of terms stored    */
static size_t cache_cap  = 0;      /* allocated capacity        */
static idx_t  m_counter   = 1;     /* current multiple of 5 to test (5*m_counter) */

static void grow_cache(void)
{
    if (cache_cap < cache_size + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *new_mem = seq_alloc(new_cap);
        if (!new_mem) exit(EXIT_FAILURE);
        for (size_t i = 0; i < cache_size; ++i) new_mem[i] = cache[i];
        seq_free(cache);
        cache = new_mem;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    while (cache_size < n) {
        idx_t candidate = 5 * m_counter;            /* 5*m_counter          */
        seq_t minus     = (seq_t)(3 * candidate - 2);
        seq_t plus      = (seq_t)(3 * candidate + 2);

        if (is_prime_big(minus) && is_prime_big(plus)) {
            grow_cache();
            cache[cache_size] = (seq_t)candidate;
            ++cache_size;
        }
        ++m_counter;
    }
    return cache[n - 1];
}
