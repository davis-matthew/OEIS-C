/*
 * A105878  Primes for which -6 is a primitive root.
 * https://oeis.org/A105878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime64, modpow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A105878: primes for which -6 is a primitive root */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime64.h"
#include "../utils/modpow64.h"

/* dynamic array of primes up to the current candidate */
static idx_t *prime_arr   = NULL;
static idx_t prime_cnt    = 0;
static idx_t prime_cap    = 0;

static void ensure_prime_cap(idx_t need)
{
    if (prime_cap < need) {
        idx_t new_cap = prime_cap ? prime_cap * 2 : 64;
        if (new_cap < need) new_cap = need;
        prime_arr = (idx_t *)realloc(prime_arr, new_cap * sizeof(idx_t));
        prime_cap = new_cap;
    }
}

static void add_prime(idx_t p)
{
    ensure_prime_cap(prime_cnt + 1);
    prime_arr[prime_cnt++] = p;
}

/* dynamic array of found terms of the sequence */
static seq_t *found_arr = NULL;
static idx_t found_cnt  = 0;
static idx_t found_cap  = 0;

static void ensure_found_cap(idx_t need)
{
    if (found_cap < need) {
        idx_t new_cap = found_cap ? found_cap * 2 : 32;
        if (new_cap < need) new_cap = need;
        found_arr = (seq_t *)realloc(found_arr, new_cap * sizeof(seq_t));
        found_cap = new_cap;
    }
}

/* test whether prime p has -6 as a primitive root (p > 6 assumed) */
static bool is_prime_primitive_root(idx_t p)
{
    uint64_t base = p - 6;          /* -6 modulo p */
    uint64_t phi  = p - 1;
    uint64_t temp = phi;

    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_arr[i];
        if (q * q > temp) break;
        if (temp % q == 0) {
            while (temp % q == 0) temp /= q;
            if (modpow64(base, (p - 1) / q, p) == 1) return false;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        if (modpow64(base, (p - 1) / temp, p) == 1) return false;
    }
    return true;
}

/* external linkage function required by checker */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset; behaviour undefined */

    static idx_t next_candidate = 2;       /* next number to test */

    while (found_cnt < n) {
        idx_t cur = next_candidate;

        /* find next prime */
        while (!is_prime64(cur)) ++cur;

        /* store the prime for factorisation of larger candidates */
        add_prime(cur);

        /* test for the primitive‑root condition */
        if (cur >= 13 && is_prime_primitive_root(cur)) {
            ensure_found_cap(found_cnt + 1);
            found_arr[found_cnt++] = (seq_t)cur;
        }

        next_candidate = cur + 1;         /* advance to next number */
    }

    return found_arr[n - 1];
}
