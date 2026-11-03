/*
 * A161471  Primes of the form k^k + k + 1.
 * https://oeis.org/A161471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   ipow, is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form k^k + k + 1 (OEIS A161471) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/is_prime_seq.h"
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

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache  = NULL;   /* array of seq_t values */
    static size_t cap    = 0;      /* current capacity of the array */
    static size_t len    = 0;      /* number of terms stored */
    static idx_t next_k  = 0;      /* next k to test (starts at 0) */

    /* If the desired term is already cached, return it */
    if (n <= len)
        return cache[n - 1];

    /* Grow the cache array if necessary */
    if (len == cap) {
        size_t new_cap = cap == 0 ? 32 : cap * 2;
        seq_t *tmp;
        /* cast the void* returned by realloc so that the code compiles
           in both C and C++ while keeping the cast explicit in C++ */
#ifdef __cplusplus
        tmp = mem_resize(seq_t, cache, new_cap);
#else
        tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp)
            exit(1);  /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Maximum value that can be held in seq_t if it is an unsigned
       integer type. For arbitrary‑precision implementations this
       value will not be used. */
    const seq_t max_val = (seq_t)-1;
    /* Search for new terms until we have the n‑th one */
    for (; len < n; ++next_k) {
        seq_t val;
        /* Compute k^k + k + 1 */
        if (sizeof(seq_t) <= 8) {           /* uint64_t or smaller */
            __uint128_t power = 1;
            idx_t k = next_k;
            /* 0^0 is treated as 1 for the first term */
            for (idx_t i = 1; i <= k; ++i) {
                /* Detect overflow before multiplication */
                if (k != 0 && power > ( (__uint128_t)max_val / k ))
                    break;
                power *= k;
            }
            /* If overflow occurred, no later term will fit in seq_t */
            if ((k != 0) && (power > max_val)) {
                break;
            }
            __uint128_t tmp_val = power + (__uint128_t)k + 1;
            if (tmp_val > (__uint128_t)max_val) {
                break;
            }
            val = (seq_t)tmp_val;
            if (is_prime_seq((uint64_t)val)) {
                cache[len++] = val;
            }
        } else {                           /* arbitrary‑precision */
            seq_t kseq = (seq_t)next_k;
            seq_t power = ipow(kseq, kseq);  /* k^k */
            val = power + kseq + (seq_t)1;
            if (is_prime_big(val)) {
                cache[len++] = val;
            }
        }
    }

    return cache[n - 1];
}
