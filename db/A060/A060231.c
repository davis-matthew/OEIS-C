/*
 * A060231  Smaller of twin primes whose middle term is a multiple of
 *          A002110(5)=2310.
 * https://oeis.org/A060231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2310·k − 1 for the n‑th integer k such that 2310·k − 1
 *            and 2310·k + 1 are both prime (the twin primes whose middle term
 *            is a multiple of 2310)
 * Range:     u8 none, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 47 terms through n = 964 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    static idx_t   cache_size = 0;   /* number of terms already found */
    static idx_t   next_k     = 1;   /* next k to be tested */
    static size_t  cache_cap  = 0;   /* array capacity (in terms) */
    static seq_t * cache      = NULL;/* array of found terms */

    /* Expand the cache until it contains at least n terms. */
    while (cache_size < n) {
        seq_t base = (seq_t)2310 * next_k;
        seq_t p    = base - 1;  /* 2310·k − 1 */
        seq_t q    = base + 1;  /* 2310·k + 1 = p + 2 */

        if (is_prime_big(p) && is_prime_big(q)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) return 0;           /* allocation failure */
                cache = tmp;
            }
            cache[cache_size++] = p;
        }
        ++next_k;
    }

    return cache[n - 1]; /* zero‑based array index */
}
