/*
 * A204901  The index j<k such that n divides s(k)-s(j), where k is the least
 *          index (A204900) for which such j exists, and s(k) denotes the k-th
 *          odd prime.
 * https://oeis.org/A204901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For A204901: for a given n ≥ 1 find the least index k such that there exists
   j < k with n | (s(k) – s(j)), where s(k) is the k‑th odd prime.
   Return that j.  Search is performed by scanning odd primes and recording
   the first index at which each residue modulo n appears. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *odd_primes      = NULL; /* stored odd primes (3,5,7,…)   */
static size_t   primes_cnt      = 0;
static size_t   primes_cap      = 0;
static uint32_t next_candidate   = 3; /* next odd number to test */

/* Ensure that at least 'need' odd primes are stored in odd_primes. */
static void
ensure_odd_primes(size_t need)
{
    if (odd_primes == NULL) {
        size_t init_cap = 16;
        odd_primes = (uint32_t *)malloc(init_cap * sizeof(uint32_t));
        primes_cap = init_cap;
    }

    while (primes_cnt < need) {
        uint32_t p = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt && (uint64_t)odd_primes[i] * odd_primes[i] <= p; ++i) {
            if (p % odd_primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                uint32_t *new_arr = (uint32_t *)realloc(odd_primes, new_cap * sizeof(uint32_t));
                if (!new_arr) return;          /* allocation failure – give up */
                odd_primes = new_arr;
                primes_cap = new_cap;
            }
            odd_primes[primes_cnt++] = p;
        }
        next_candidate += 2; /* test next odd number */
    }
}

/* Public entry point requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined, return 0 safely */

    /* We need at least n+1 odd primes to guarantee a repeat of a residue. */
    ensure_odd_primes((size_t)n + 1);

    /* Array to remember the first index where each residue appeared.
       Indexes are 1‑based: the first odd prime (3) has index 1. */
    size_t *first_occ = (size_t *)malloc((size_t)n * sizeof(size_t));
    if (!first_occ)
        return (seq_t)0;                 /* unlikely allocation failure */

    for (size_t i = 0; i < (size_t)n; ++i)
        first_occ[i] = SIZE_MAX;

    for (size_t k = 0; k < primes_cnt; ++k) {
        uint32_t prime = odd_primes[k];
        size_t    r = (size_t)(prime % n);
        if (first_occ[r] == SIZE_MAX) {
            first_occ[r] = k + 1;        /* store 1‑based index */
        } else {
            size_t j = first_occ[r];
            free(first_occ);
            return (seq_t)j;             /* answer found */
        }
    }

    free(first_occ);                     /* should never be reached */
    return (seq_t)0;
}
