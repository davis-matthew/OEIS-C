/*
 * A230359  Prime numbers p such that their Fibonacci entry points are less
 *          than p+1.
 * https://oeis.org/A230359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 5258, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230359: primes p whose Fibonacci entry point A001177(p)
   is strictly less than p+1. Computed by searching primes,
   checking the entry point via fast doubling with modular arithmetic.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t, uint64_t, seq_t) */

/* Fast multiplication modulo mod (handles any seq_t width via 128‑bit temp). */
static inline seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* Fast doubling Fibonacci: returns (F(n),F(n+1)) modulo mod */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1, seq_t mod)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b, mod);

    seq_t two_b = (b << 1) % mod;
    seq_t temp = (two_b >= a) ? (two_b - a) : (mod + two_b - a);
    c = mul_mod_seq(a, temp, mod);
    d = (mul_mod_seq(a, a, mod) + mul_mod_seq(b, b, mod)) % mod;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* F(n) modulo mod */
static seq_t fib_mod(idx_t n, seq_t mod)
{
    seq_t f, g;
    fib_pair(n, &f, &g, mod);
    return f;
}

/* Smallest n>0 such that F(n) ≡ 0 (mod p) */
static uint64_t fibonacci_entry_point(uint64_t p)
{
    if (p == 5) return 5;          /* known entry point */

    /* Legendre symbol (5|p) via Euler's criterion */
    seq_t s = mod_pow((seq_t)5, (p - 1) >> 1, (seq_t)p);
    uint64_t s_mod = (uint64_t)s;   /* should be 1 or p-1 */

    uint64_t D = (s_mod == 1) ? (p - 1) : (p + 1);

    /* Enumerate divisors of D */
    uint64_t small[2048], large[2048];
    size_t small_cnt = 0, large_cnt = 0;
    for (uint64_t d = 1; d * d <= D; ++d) {
        if (D % d == 0) {
            small[small_cnt++] = d;
            if (d != D / d) large[large_cnt++] = D / d;
        }
    }

    seq_t mod = (seq_t)p;

    /* Check divisors in ascending order */
    for (size_t i = 0; i < small_cnt; ++i)
        if (fib_mod(small[i], mod) == 0) return small[i];

    for (size_t i = large_cnt; i-- > 0;)
        if (fib_mod(large[i], mod) == 0) return large[i];

    return D;   /* should never reach here */
}

/* Sequence state */
static seq_t *valid_primes = NULL;   /* array of primes satisfying the property */
static size_t  valid_len   = 0;      /* number of stored primes */
static size_t  valid_cap   = 0;      /* current capacity of the array */
static uint64_t next_candidate = 3;  /* next odd number to test */

/* Ensure we have at least n primes in valid_primes */
static void ensure_up_to(idx_t n)
{
    while (valid_len < n) {
        uint64_t cand = next_candidate;

        if (is_prime(cand)) {
            uint64_t entry = fibonacci_entry_point(cand);
            if (entry < cand + 1) {
                /* add cand to list */
                if (valid_len == valid_cap) {
                    valid_cap = (valid_cap == 0) ? 4 : valid_cap << 1;
                    valid_primes = (seq_t *)realloc(valid_primes, valid_cap * sizeof(seq_t));
                }
                valid_primes[valid_len++] = (seq_t)cand;
            }
        }
        /* advance to next odd candidate */
        next_candidate += 2;
    }
}

/* External function required by the build */
seq_t a(idx_t n)
{
    /* Calls may be out of order; ensure state is ready */
    ensure_up_to(n);
    return valid_primes[n - 1];
}
