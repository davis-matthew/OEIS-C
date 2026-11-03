/*
 * A167833  Three-times-isolated primes: primes p such that neither p+-2, p+-4
 *          nor p+-6 is prime.
 * https://oeis.org/A167833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1741, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 61 terms through n = 2196 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function returns the nth three‑times‑isolated prime.
   A prime p is three‑times‑isolated if none of p±2, p±4, p±6
   are prime.  The implementation searches primes incrementally
   and memoises the results so that subsequent calls do not
   recompute earlier terms.  The routine is efficient enough
   for n up to about 10 000. 
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
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
    /* For n < 1 the sequence is undefined; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of cached terms */
    static size_t cap   = 0;   /* capacity of cache */
    /* next candidate to test (2, then odd numbers) */
    static seq_t next = 2;

    while (len < n)
    {
        seq_t cand = next;
        bool ok = false;

        if (cand == (seq_t)2) {
            /* 2 is isolated by definition */
            ok = true;
        } else if (is_prime_big(cand)) {
            /* check isolation */
            if (!is_prime_big(cand + (seq_t)2) &&
                cand > (seq_t)2   && !is_prime_big(cand - (seq_t)2) &&
                !is_prime_big(cand + (seq_t)4)             &&
                cand > (seq_t)4   && !is_prime_big(cand - (seq_t)4) &&
                !is_prime_big(cand + (seq_t)6)             &&
                cand > (seq_t)6   && !is_prime_big(cand - (seq_t)6))
            {
                ok = true;
            }
        }

        if (ok)
        {
            /* grow cache if needed */
            if (len + 1 > cap)
            {
                size_t new_cap = cap ? (cap << 1) : 4;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = cand;
        }

        /* advance to next candidate: start with 3, then odd numbers */
        if (next == (seq_t)2)
            next = (seq_t)3;
        else
            next += (seq_t)2;
    }

    return cache[n - (idx_t)1];
}
