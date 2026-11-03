/*
 * A163155  Primes of the form A001654(k) - 2.
 * https://oeis.org/A163155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163155: primes of the form F(k) * F(k+1) – 2,
   where F(k) is the k-th Fibonacci number (F(1)=1, F(2)=1). */

#include <stdlib.h>
#include <string.h>
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
    /* static state: primes cache and Fibonacci generator */
    static seq_t *prime_cache = NULL;
    static size_t prime_cnt  = 0;
    static size_t prime_cap  = 0;

    static seq_t fib_k    = (seq_t)1;  /* F(k)   */
    static seq_t fib_k1   = (seq_t)1;  /* F(k+1) */
    static idx_t  fib_idx = 1;         /* current Fibonacci index (k) */

    /* If the requested value is already cached, return it. */
    if (n > 0 && n <= prime_cnt)
        return prime_cache[n - 1];

    /* Ensure cache capacity. */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_cache = mem_new(seq_t, prime_cap);
    }

    /* Generate primes until we have at least n of them. */
    while (prime_cnt < n) {
        /* Check if F(k) * F(k+1) - 2 is prime. (All values are seq_t) */
        seq_t prod = fib_k * fib_k1;
        seq_t val  = prod - 2;

        if (is_prime_big(val)) {
            /* Grow cache if necessary. */
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap * 2;
                seq_t *new_cache =
                    mem_new(seq_t, new_cap);
                mem_copy(new_cache, prime_cache, sizeof(seq_t) * prime_cap);
                mem_free(prime_cache);
                prime_cache = new_cache;
                prime_cap   = new_cap;
            }
            prime_cache[prime_cnt++] = val;
        }

        /* Advance Fibonacci numbers. */
        seq_t tmp = fib_k + fib_k1;
        fib_k    = fib_k1;
        fib_k1   = tmp;
        ++fib_idx;
    }

    return prime_cache[n - 1];
}
