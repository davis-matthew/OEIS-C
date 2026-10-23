/*
 * A036957  Primes with digits (0,...,4) taken as base 5 and converted to base
 *          10.
 * https://oeis.org/A036957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal conversion of base‑5 representation of the nth
 *            prime * whose decimal digits are all in {0,1,2,3,4}. * The
 *            function is computed by generating primes in order and keeping
 *            those * that satisfy the digit condition
 * Range:     u8 n <= 40, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static seq_t *vals = NULL;       /* populated a(1)…a(len) */
static size_t len  = 0;
static size_t cap  = 0;

static uint64_t *primes = NULL;  /* list of primes found so far */
static size_t primes_len = 0;
static size_t primes_cap = 0;

static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

static void grow_primes(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        *cap = new_cap;
    }
}

/* Ensure that the sequence contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while (len < n) {
        /* Find next prime */
        static uint64_t candidate = 2;
        while (true) {
            if (is_prime(candidate)) break;
            candidate++;   /* natural counter, safe for up to 64‑bit */
        }

        /* Test decimal digits */
        uint64_t temp = candidate;
        bool ok = true;
        uint8_t digits[64];
        size_t dlen = 0;
        while (temp > 0) {
            uint8_t d = temp % 10;
            if (d > 4) { ok = false; break; }
            digits[dlen++] = d;
            temp /= 10;
        }
        if (ok) {
            /* Convert to base‑5 decimal value */
            seq_t val = 0;
            for (size_t i = dlen; i > 0; --i) {
                val = val * 5 + digits[i - 1];
            }
            grow_seq(&vals, &cap, len + 1);
            vals[len++] = val;
        }

        /* Record the prime for future divisor checks */
        grow_primes(&primes, &primes_cap, primes_len + 1);
        primes[primes_len++] = candidate;

        candidate++;  /* next candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but safe */
    ensure_terms(n);
    return vals[n - 1];
}
