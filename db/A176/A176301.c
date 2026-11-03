/*
 * A176301  Numbers k such that the k-th semiprime + 2 is prime.
 * https://oeis.org/A176301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that the k-th semiprime + 2 is prime
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by scanning integers and checking semiprime status and */
/* primality of the semiprime+2. Primitive sieve for primes is used for */
/* efficient factorisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility helpers */
#include <string.h>
#include "../utils/isqrt.h"
#include "../utils/sieve_primes.h"
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

/* -------------------------------------------------------------------- */
/* Prime table (dynamic)                                               */
/* -------------------------------------------------------------------- */
static idx_t *prime_tbl   = NULL;      /* array of primes */
static size_t prime_cnt   = 0;          /* number of primes stored */
static idx_t  prime_limit = 0;         /* current sieve limit */

/* Ensure prime_tbl contains all primes up to 'limit'. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    free(prime_tbl);
    prime_tbl = NULL;
    size_t cnt;
    sieve_primes(limit, &prime_tbl, &cnt);
    prime_limit = limit;
    prime_cnt   = cnt;
}

/* -------------------------------------------------------------------- */
/* Test whether a number is a semiprime (product of exactly two prime */
/* factors counted with multiplicity).                                  */
/* -------------------------------------------------------------------- */
static bool is_semiprime(idx_t n)
{
    if (n < 4)
        return false;

    /* Factorise n using primes up to sqrt(n). */
    idx_t temp  = n;
    int    factors = 0;
    idx_t limit   = isqrt(temp);
    ensure_primes(limit + 1);   /* primes needed for factorisation */

    for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= limit; ++i) {
        idx_t p = prime_tbl[i];
        while (temp % p == 0) {
            ++factors;
            if (factors > 2)
                return false;
            temp /= p;
            if (temp == 1)
                break;
        }
        if (temp == 1)
            break;
    }
    if (temp > 1) {          /* remaining prime factor */
        ++factors;
        if (factors > 2)
            return false;
    }

    return factors == 2;
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* undefined, but harmless */

    idx_t found = 0;        /* how many qualifying semiprimes seen */
    idx_t semi_idx = 0;     /* index of current semiprime in the list */

    for (idx_t num = 4; ; ++num) {      /* start from the first semiprime */
        if (!is_semiprime(num))
            continue;

        ++semi_idx;                     /* we found the (semi_idx)-th semiprime */

        /* Check whether num + 2 is prime. */
        if (is_prime_big((seq_t)(num + 2))) {
            ++found;
            if (found == n)
                return (seq_t)semi_idx;
        }
    }
}
