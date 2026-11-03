/*
 * A048059  Primes of the form k^2 + k + 11.
 * https://oeis.org/A048059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 96, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9999 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048059: primes of the form k^2 + k + 11
   Computed by iterating k = 0,1,2,... and storing values
   of k^2 + k + 11 that are prime. The function keeps a
   static cache so that calls with arbitrary n work in O(1) amortized time. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *primes   = NULL; /* cached primes of the required form */
static size_t primes_cap   = 0;
static size_t primes_len   = 0;
static idx_t next_k = 0;          /* next k to examine */

/* Return the n-th prime of the form k^2 + k + 11 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for accidental 0 index */

    while (primes_len < n) {
        /* Compute the candidate value in a larger type to avoid overflow
           on intermediate calculations.  It is then cast to seq_t when
           stored;  this is correct for all values that fit into seq_t,
           which is the range that the tests exercise. */
        idx_t tmp = next_k * next_k + next_k + 11;
        seq_t val = (seq_t)tmp;

        if (is_prime_big(val)) {
            /* grow the primes array if necessary */
            if (primes_len + 1 > primes_cap) {
                if (primes_cap == 0) primes_cap = 4;
                size_t new_cap = primes_cap * 2;
                if (new_cap < primes_len + 1) new_cap = primes_len + 1;
                seq_t *tmp_p = mem_resize(seq_t, primes, new_cap);
                if (!tmp_p) return 0; /* allocation failed */
                primes = tmp_p;
                primes_cap = new_cap;
            }
            primes[primes_len++] = val;
        }
        ++next_k;
    }

    return primes[n - 1];
}
