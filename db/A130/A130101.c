/*
 * A130101  Primes prime(k) such that 10*prime(k) + prime(k+1) is not prime.
 * https://oeis.org/A130101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130101: The nth prime p such that 10p + nextprime(p) is composite.
   The function lazily generates primes using next_prime() and caches
   the qualifying ones. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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
    /* Static state retained across calls. */
    static uint64_t last_checked = 1;   /* start before the first prime */
    static size_t count = 0;            /* number of stored results      */
    static seq_t *cache = NULL;         /* array of qualifying primes    */
    static size_t cap = 0;              /* allocated capacity of cache   */

    /* Ensure we have at least n results. */
    while (count < (size_t)n) {
        /* Get next prime p. */
        last_checked = next_prime(last_checked);
        uint64_t p = last_checked;
        /* Find the next prime after p. */
        uint64_t q = next_prime(p);

        /* Compute 10*p + q as seq_t (enough to hold the value). */
        seq_t combo = (seq_t)p * 10 + (seq_t)q;

        /* Store p if the test fails (i.e., combo is composite). */
        if (!is_prime_big(combo)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 128;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (!tmp) {
                    mem_free(cache);
                    abort();                 /* unrecoverable allocation error */
                }
                cache = tmp;
            }
            cache[count++] = (seq_t)p;
        }
    }

    /* Return the nth element (offset 1). */
    return cache[count - 1];
}
