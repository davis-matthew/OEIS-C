/*
 * A082074  One quarter of first differences of primes of the form 4*k+1
 *          (A002144).
 * https://oeis.org/A082074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A002144(n+1) - A002144(n)) / 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002144: primes congruent to 1 modulo 4.  */

/* This implementation lazily generates all primes and
   extracts those ≡1 (mod 4).  It stores prime lists in
   dynamically growing arrays, avoiding any external
   dependencies.  */

#include <stdlib.h>
#include <stdbool.h>

/* Global state for prime generation */
static idx_t *primes      = NULL;   /* all primes */
static size_t primes_cap  = 0;
static size_t primes_len  = 0;

static idx_t *primes1     = NULL;   /* primes ≡1 (mod 4) */
static size_t primes1_cap = 0;
static size_t primes1_len = 0;

static idx_t last_checked   = 1;    /* last number examined for primality */

/* Append a prime to the global lists */
static void add_prime(idx_t p)
{
    /* grow all‑primes vector if needed */
    if (primes_len == primes_cap) {
        size_t newcap = primes_cap ? primes_cap << 1 : 8;
        primes = (idx_t*)realloc(primes, newcap * sizeof(idx_t));
        primes_cap = newcap;
    }
    primes[primes_len++] = p;

    /* add to 1‑mod‑4 list if appropriate */
    if (p % 4 == 1) {
        if (primes1_len == primes1_cap) {
            size_t newcap = primes1_cap ? primes1_cap << 1 : 8;
            primes1 = (idx_t*)realloc(primes1, newcap * sizeof(idx_t));
            primes1_cap = newcap;
        }
        primes1[primes1_len++] = p;
    }
}

/* Generates primes until we have at least (need+1) 1‑mod‑4 primes
   (so that a(need) can be computed).  */
static void ensure_primes1_count(idx_t need)
{
    while (primes1_len <= need) {
        idx_t cand = last_checked + 1;
        last_checked = cand;

        bool is_prime = true;
        if (cand < 2)
            is_prime = false;
        else if (cand == 2) {
            is_prime = true;
        } else if (cand % 2 == 0) {
            is_prime = false;
        } else {
            /* trial division using known primes up to sqrt(cand) */
            for (size_t i = 0; i < primes_len; ++i) {
                idx_t p = primes[i];
                if (p > cand / p)   /* safer than p*p > cand */
                    break;
                if (cand % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime)
            add_prime(cand);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined below offset, safe return */

    ensure_primes1_count(n);       /* need at least n+1 elements */

    /* primes1[0] = A002144(1), so difference for a(n) is
       primes1[n] - primes1[n-1]  (both 1‑mod‑4 primes) */
    seq_t diff = (seq_t)(primes1[n] - primes1[n - 1]);

    return diff / 4;
}
