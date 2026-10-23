/*
 * A045701  Number of ways n can be written as a sum of a square of a prime
 *          and a cube of a prime.
 * https://oeis.org/A045701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{ (p,q) : p,q prime and p^2 + q^3 = n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by generating primes up to sqrt(n) and cbrt(n),
   then testing all prime pairs */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- helper routines for integer arithmetic ----------
   (no external utilities are required) */

static idx_t floor_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

static idx_t cube_root(idx_t n)
{
    if (n < 8) return 1;                /* floor(n^(1/3)) for tiny n */
    idx_t hi = 1;
    while ((unsigned __int128)hi * hi * hi <= n) hi <<= 1; /* hi^3 > n */
    idx_t lo = hi >> 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 cube = (unsigned __int128)mid * mid * mid;
        if (cube == n) return mid;
        if (cube < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* ---------- prime generator with simple trial division ----------
   primes are stored in a static table that grows only as needed   */

static idx_t      primes_limit = 1; /* maximum prime already generated */
static size_t     primes_cnt   = 0; /* number of primes stored      */
static size_t     primes_cap   = 0; /* allocated capacity           */
static uint64_t  *primes       = NULL; /* array of primes             */

static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= primes_limit) return;

    /* grow capacity */
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    while (new_cap < limit + 1) new_cap *= 2;
    if (primes_cap < new_cap) {
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
        primes       = tmp;
        primes_cap   = new_cap;
    }

    /* extend the prime table */
    for (idx_t cand = primes_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue; /* 0 and 1 are not prime */
        bool is_prime = true;
        idx_t sq = floor_sqrt(cand);
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t p = primes[i];
            if (p > sq) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) primes[primes_cnt++] = cand;
    }
    primes_limit = limit;
}

/* ---------- main sequence function ----------
   a(idx_t n) returns the number of representations.  ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    idx_t sq_lim  = floor_sqrt(n);
    idx_t cb_lim  = cube_root(n);
    idx_t prime_lim = (sq_lim > cb_lim) ? sq_lim : cb_lim;

    ensure_primes_up_to(prime_lim);

    seq_t count = 0;
    unsigned __int128 un = (unsigned __int128)n;

    for (size_t i = 0; i < primes_cnt && primes[i] <= sq_lim; ++i) {
        unsigned __int128 sq = (unsigned __int128)primes[i] * primes[i];
        if (sq > un) break;
        for (size_t j = 0; j < primes_cnt && primes[j] <= cb_lim; ++j) {
            unsigned __int128 cu = (unsigned __int128)primes[j] *
                                  primes[j] * primes[j];
            if (cu > un) break;
            if (sq + cu == un) ++count;
        }
    }
    return count;
}
