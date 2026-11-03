/*
 * A221220  Numbers with more than one prime factor such that concatenation of
 *          its prime factors (without multiplicity) is a prime.
 * https://oeis.org/A221220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS A221220: numbers with ≥2 distinct prime factors whose
   concatenation of those factors (in ascending order) is prime. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include <math.h>
#include "../utils/num_digits0.h"
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
    /* Static state: list of found terms, list of known primes. */
    static seq_t *terms   = NULL;
    static size_t  terms_len = 0, terms_cap = 0;
    static uint64_t *primes   = NULL;
    static size_t  primes_len = 0, primes_cap = 0;
    static uint64_t cur = 2;                     /* start from 2 */

    if (n == 0) return 0;                       /* offset starts at 1 */

    /* Grow the terms list until we have at least n entries. */
    while (terms_len < (size_t)n) {

        uint64_t temp = cur;
        uint64_t distinct[16];
        size_t dp = 0;

        /* Factor cur using known primes. */
        for (size_t pi = 0; pi < primes_len &&
            (uint64_t)primes[pi] * primes[pi] <= temp; ++pi) {
            uint64_t p = primes[pi];
            if (temp % p == 0) {
                distinct[dp++] = p;
                while (temp % p == 0) temp /= p;
            }
        }

        if (temp > 1)                    /* remaining prime factor */
            distinct[dp++] = temp;

        /* Evaluate if cur qualifies. */
        bool qualifies = false;
        if (dp >= 2) {
            seq_t concat = 0;
            for (size_t i = 0; i < dp; ++i) {
                uint64_t p   = distinct[i];
                idx_t    dks = (idx_t)num_digits0(p);
                seq_t multiplier = 1;
                for (idx_t r = 0; r < dks; ++r) multiplier *= 10;
                concat = concat * multiplier + (seq_t)p;
            }
            if (is_prime_big(concat))
                qualifies = true;
        }

        if (qualifies) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = (seq_t)cur;
        }

        /* If cur itself is prime, remember it for future factorizations. */
        if (dp == 1 && distinct[0] == cur) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint64_t *)realloc(primes,
                                            primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = cur;
        }

        ++cur;    /* move to the next integer to test */
    }

    return terms[n - 1];
}
