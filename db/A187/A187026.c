/*
 * A187026  Least m such that prime(n) divides (prime(m) + prime(m+1))/2.
 * https://oeis.org/A187026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least m such that prime(n) divides
 *            (prime(m)+prime(m+1))/2 * * The implementation keeps a dynamic
 *            list of primes. For each call it * ensures that prime(n),
 *            prime(m) and prime(m+1) are available and * searches m until the
 *            divisibility condition holds. * It works for any seq_t (fixed
 *            wid...
 * Range:     u8 n <= 9, u16 n <= 489, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* --- Prime generation utilities ------------------------------------- */

static uint64_t *primes        = NULL;   /* array of primes (uint64_t)   */
static size_t   primes_used   = 0;      /* number of primes stored      */
static size_t   primes_cap    = 0;      /* allocated capacity           */
static uint64_t next_candidate = 3ULL;  /* next odd number to test      */

/* grow the prime array to at least new_cap */
static void grow_primes(size_t new_cap)
{
    if (new_cap <= primes_cap) return;
    size_t sz = primes_cap ? primes_cap : 16;
    while (sz < new_cap) sz <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, sz * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes     = tmp;
    primes_cap = sz;
}

/* ensure that at least 'need' primes are available (need >= 1) */
static void ensure_primes_count(idx_t need)
{
    if (need == 0) return;

    if (primes_used == 0) {            /* initialise with first prime */
        grow_primes(16);
        primes[0]          = 2ULL;
        primes_used        = 1;
        next_candidate     = 3ULL;
    }

    while ((idx_t)primes_used < need) {
        uint64_t cand     = next_candidate;
        bool     is_prime = true;
        uint64_t sqrt_cand = (uint64_t)sqrt((double)cand);

        for (size_t i = 0; i < primes_used && primes[i] <= sqrt_cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            grow_primes(primes_used + 1);
            primes[primes_used++] = cand;
            next_candidate        = cand + 2ULL;
        } else {
            next_candidate += 2ULL;
        }
    }
}

/* --- Sequence function --------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */

    ensure_primes_count(n);                 /* ensure prime(n) exists */
    uint64_t p_n = primes[n - 1];

    seq_t mod = (seq_t)2 * (seq_t)p_n;      /* 2 * prime(n) */

    idx_t m = 2;                            /* start search from m=2 */
    for (;;) {
        /* ensure prime(m) and prime(m+1) are available */
        ensure_primes_count(m + 1);

        uint64_t p_m   = primes[m - 1];
        uint64_t p_next = primes[m];

        seq_t sum = (seq_t)p_m + (seq_t)p_next;

        if (sum % mod == 0) return (seq_t)m;

        ++m;
    }
}
