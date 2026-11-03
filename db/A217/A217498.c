/*
 * A217498  Primes of the form 2*n^2 + 58*n + 27.
 * https://oeis.org/A217498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 55 terms through n = 2939 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes of the form 2*k^2 + 58*k + 27.
   Return the n‑th such prime (OEIS index starts at 1). */
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

/* static state used to memoise previously found primes     */
static seq_t *prime_cache   = NULL; /* array of primes in order  */
static size_t prime_len    = 0;      /* number of primes stored   */
static size_t prime_cap    = 0;      /* capacity of prime_cache   */
static uint64_t next_k     = 0;      /* next k value to test      */

/* Return the n‑th prime of the quadratic 2*k^2 + 58*k + 27 */
seq_t a(idx_t n)
{
    if (n == 0)                          /* undefined for 0, return 0   */
        return (seq_t)0;

    while (prime_len < n) {              /* generate more primes  */
        /* compute 2*k^2 + 58*k + 27 in seq_t arithmetic */
        seq_t k  = (seq_t)next_k;                            /* cast once */
        seq_t val = ((seq_t)2 * k * k) + ((seq_t)58 * k) + (seq_t)27;

        if (is_prime_big(val)) {          /* check primality     */
            if (prime_cap == 0) {          /* first allocation   */
                prime_cap = 16;
                prime_cache = mem_new(seq_t, prime_cap);
            } else if (prime_len >= prime_cap) { /* grow            */
                prime_cap *= 2;
                prime_cache = mem_resize(seq_t, prime_cache, prime_cap);
            }
            prime_cache[prime_len++] = val;
        }
        ++next_k;                         /* advance to next k   */
    }
    return prime_cache[n - 1];            /* return stored prime */
}
