/*
 * A093326  Duplicate of A001359.
 * https://oeis.org/A093326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A093326: primes p such that (p + nextprime(p)) / 2 + 1 is prime
   (2 is omitted to match the OEIS listed terms). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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

/* static cache of qualifying primes */
static seq_t  *qual = NULL;    /* array holding the primes that satisfy the condition */
static size_t   qual_cap = 0;   /* allocated capacity */
static size_t   qual_len = 0;   /* number of primes stored */
static uint64_t curr_prime = 1; /* last prime examined (start at 1 so first next_prime is 2) */

/* make sure 'qual' has at least 'needed' slots */
static void
ensure_cap(size_t needed)
{
    if (qual_cap < needed) {
        size_t new_cap = (qual_cap == 0) ? 16 : qual_cap << 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, qual, new_cap);
        if (!tmp) abort();          /* allocation failure */
        qual = tmp;
        qual_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safeguard for unexpected call */

    /* extend the cache until it contains at least n qualifying primes */
    while (qual_len < (size_t)n) {
        /* advance to the next prime */
        curr_prime = next_prime(curr_prime);
        /* skip the prime 2 to match the OEIS sequence listing */
        if (curr_prime == 2) continue;

        uint64_t nextp = next_prime(curr_prime);

        /* compute (p + nextp) / 2 + 1, using 64‑bit arithmetic */
        uint64_t mid_plus = (curr_prime + nextp) / 2 + 1;

        /* test primality of the result */
        if (is_prime_big((seq_t)mid_plus)) {
            ensure_cap(qual_len + 1);
            qual[qual_len++] = (seq_t)curr_prime;
        }
    }

    /* return the (n‑1)-th qualifying prime */
    return qual[(size_t)n - 1];
}
