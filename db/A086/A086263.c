/*
 * A086263  Smaller of two consecutive squarefree numbers having equal numbers
 *          of prime factors.
 * https://oeis.org/A086263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4996, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = n-th integer k such that k and k+1 are square‑free,
 *         and Ω(k) = Ω(k+1)  (Ω = number of distinct prime factors).
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h" /* build_primes_upto() */
#include "../utils/int_sqrt.h"          /* int_sqrt() */

/* Cache of sequence values */
static seq_t *cache     = NULL;   /* cached terms                    */
static size_t clen      = 0;      /* number of terms cached          */
static size_t ccap      = 0;      /* allocated capacity of cache     */

/* Prime table used for factorisation */
static uint64_t *primes = NULL;   /* list of primes for factorisation */
static size_t   pcnt   = 0;       /* number of primes in list         */
static idx_t    plimit = 0;       /* largest prime pre‑computed       */

/* Ensure prime list contains all primes ≤ needed. */
static void ensure_primes(idx_t needed)
{
    if (plimit >= needed)
        return;
    free(primes);                               /* discard old list if any      */
    build_primes_upto(needed, &primes, &pcnt);   /* primes <= needed            */
    plimit = needed;
}

/* Factorise n (≤ 2^64‑1).  Returns
 *    w  = number of distinct prime factors (Ω)
 *    sf = true iff n is square‑free.
 */
static void factorise(idx_t n, seq_t *omega, bool *squarefree)
{
    seq_t w  = 0;
    bool  sf = true;
    idx_t tmp = n;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p   = primes[i];
        idx_t    p64 = (idx_t)p;

        /* stop when p² > tmp  */
        if (p64 * p64 > tmp)
            break;

        if (tmp % p == 0) {
            int once = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++once;
                if (once > 1)
                    sf = false;   /* found square factor */
            }
            ++w;                 /* counted a distinct prime */
        }
    }
    if (tmp > 1)
        ++w;                     /* remaining prime factor */

    *omega      = w;
    *squarefree = sf;
}

/* Generate sequence terms up to index n if not already cached. */
static void generate_terms(idx_t n)
{
    static idx_t candidate = 2;                     /* next integer to test */

    while (clen < n) {
        /* Ensure we have primes up to sqrt(candidate+1) */
        idx_t required = int_sqrt(candidate + 1) + 1;
        ensure_primes(required);

        seq_t w1, w2;
        bool sf1, sf2;

        factorise(candidate,   &w1, &sf1);
        factorise(candidate+1, &w2, &sf2);

        if (sf1 && sf2 && w1 == w2) {
            /* Append current to cache */
            if (clen == ccap) {
                size_t newcap = (ccap == 0) ? 64 : ccap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) { free(cache); free(primes); exit(1); }
                cache = tmp;
                ccap = newcap;
            }
            cache[clen++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* External API ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset is 1, guard against 0 */

    if (clen < n)
        generate_terms(n);
    return cache[n - 1];
}
