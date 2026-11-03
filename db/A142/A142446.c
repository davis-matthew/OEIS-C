/*
 * A142446  Primes congruent to 38 mod 49.
 * https://oeis.org/A142446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 879 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime congruent to 38 modulo 49 by testing
   successive candidates. */

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

/* Return the nth prime with residue 38 modulo 49 (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *primes      = NULL;   /* array of primes */
    static size_t primes_len  = 0;      /* number of primes stored */
    static size_t primes_cap  = 0;      /* current capacity */
    static seq_t next_candidate = 38;   /* next number ≡ 38 (mod 49) */

    /* Grow the cache until it contains at least n primes. */
    while (primes_len < (size_t)n)
    {
        /* Advance until the next prime is found. */
        while (!is_prime_big(next_candidate))
            next_candidate += 49;

        /* Grow the array if necessary. */
        if (primes_len == primes_cap)
        {
            size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
            seq_t *tmp = mem_resize(seq_t, primes, new_cap);
            if (!tmp) abort();              /* allocation failure */
            primes = tmp;
            primes_cap = new_cap;
        }

        /* Store the discovered prime. */
        primes[primes_len++] = next_candidate;

        /* Advance to the next candidate with the same residue. */
        next_candidate += 49;
    }

    /* Return the requested element (1‑based index). */
    return primes[n - 1];
}
