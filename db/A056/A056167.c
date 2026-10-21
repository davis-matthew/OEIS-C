/*
 * A056167  Numbers k such that k! is not divisible by the square of (f+1)!,
 *          where f = floor(k/2).
 * https://oeis.org/A056167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = k, the n‑th integer k ≥ 2 for which
 *   (k!)/((floor(k/2)+1)!)² is not an integer.
 *   The sequence is found by searching increasing k.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*----- static data for cache and prime list --------------------------------*/
static seq_t *seq_cache = NULL;     /* array of found terms            */
static size_t seq_len   = 0;        /* number of terms cached          */
static size_t seq_cap   = 0;        /* allocated capacity              */
static idx_t  next_k    = 2;        /* next integer k to test (≥2)     */

static idx_t *prime_list  = NULL;  /* list of primes generated so far */
static size_t prime_count = 0;      /* number of primes stored         */
static size_t prime_cap   = 0;      /* capacity of prime_list          */

/*----- utilities -----------------------------------------------------------*/

/* resize prime_list if needed */
static void ensure_prime_cap(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    if (new_cap < needed) new_cap = needed;
    idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
    if (!tmp) exit(EXIT_FAILURE);
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* generate all primes up to and including `limit` */
static void ensure_primes_up_to(idx_t limit)
{
    /* start searching from the integer after the last known prime */
    idx_t cand = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    while (cand <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_prime_cap(prime_count + 1);
            prime_list[prime_count++] = cand;
        }
        ++cand;
    }
}

/* Legendre exponent of prime p in m! */
static idx_t legendre_exp(idx_t m, idx_t p)
{
    idx_t exp = 0;
    idx_t pow = p;
    while (pow <= m) {
        exp += m / pow;
        if (pow > m / p) break;          /* prevent unsigned overflow */
        pow *= p;
    }
    return exp;
}

/* test whether k satisfies the definition (square divisor condition) */
static bool qualifies(idx_t k, idx_t f)
{
    idx_t limit = f + 1;                     /* (f+1)! */
    for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
        idx_t p   = prime_list[i];
        idx_t ek  = legendre_exp(k, p);
        idx_t ef  = legendre_exp(limit, p);
        if (ek < 2 * ef) return true;           /* square not dividing */
    }
    return false;
}

/*----- public sequence function --------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0ULL;                    /* not used, offset is 1 */

    /* grow cache to capacity at least n */
    if (seq_cap < n) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* generate terms until we have at least n terms */
    while (seq_len < n) {
        idx_t f = next_k >> 1;                    /* floor(k/2) */
        ensure_primes_up_to(f + 1);
        if (qualifies(next_k, f))
            seq_cache[seq_len++] = (seq_t)next_k;
        ++next_k;
    }

    return seq_cache[n - 1];
}
