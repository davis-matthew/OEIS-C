/*
 * A079707  In prime factorization of n replace odd primes with their prime
 *          predecessor.
 * https://oeis.org/A079707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π (p_i == 2 ? 2 : prevprime(p_i))^e_i where the product
 *            runs over the prime factorisation n = Π p_i^e_i and prevprime(p)
 *            is the largest prime < p. This implements OEIS A079707
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Integer square‑root (no floating‑point, works for any idx_t).        */
static idx_t floor_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod == n) return mid;
        if (prod < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* --------------------------------------------------------------------- */
/*  Prime storage and generation                                         */
static idx_t *primes = NULL;   /* dynamic array of primes  */
static size_t primes_cnt = 0;  /* number of primes stored  */
static size_t primes_cap = 0;  /* current capacity        */

/* Grow the prime array to at least 'need' elements.        */
static void expand_primes_cap(size_t need)
{
    if (primes_cap < need) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* Ensure that all primes ≤ limit are generated and stored.  */
static void ensure_primes_upto(idx_t limit)
{
    if (primes == NULL) {                 /* first call */
        expand_primes_cap(limit + 2);
        primes[0] = 2;
        primes_cnt = 1;
        if (limit < 2) return;
    }

    idx_t last = primes[primes_cnt - 1];
    if (limit <= last) return;

    idx_t candidate = last + 1;
    if (candidate <= 2) candidate = 3;
    if ((candidate & 1) == 0) candidate++;   /* start with odd */

    for (; candidate <= limit; candidate += 2) {
        idx_t root = floor_sqrt(candidate);
        bool is_p = true;
        for (size_t i = 0; i < primes_cnt && primes[i] <= root; ++i) {
            if (candidate % primes[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            expand_primes_cap(primes_cnt + 1);
            primes[primes_cnt++] = candidate;
        }
    }
}

/* Binary search to find the largest prime < p (p is assumed prime).  */
static idx_t prev_prime(idx_t p)
{
    if (p <= 2) return 2;               /* prevprime(2) = 2 by convention */
    size_t lo = 0, hi = primes_cnt - 1;
    size_t idx = 0;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) {
            idx = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return primes[idx];
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                               */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 1;            /* by definition a(1)=1 */

    seq_t res = 1;
    idx_t tmp = n;

    /* Generate primes up to sqrt(tmp) to factorise. */
    ensure_primes_upto(floor_sqrt(tmp));

    /* Trial division using the prime list. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t mult;
            if (p == 2) {
                mult = (seq_t)2;
            } else {
                mult = (seq_t)prev_prime(p);   /* replace by predecessor prime */
            }
            for (idx_t k = 0; k < exp; ++k) res *= mult;
        }
    }

    /* If a prime factor > sqrt(original n) remains, it is itself a prime. */
    if (tmp > 1) {
        ensure_primes_upto(tmp);
        seq_t mult;
        if (tmp == 2) {
            mult = (seq_t)2;
        } else {
            mult = (seq_t)prev_prime(tmp);
        }
        res *= mult;
    }

    return res;
}
