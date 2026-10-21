/*
 * A067827  Even numbers k such that k/2 is nonprime and sigma(k+1) >
 *          sigma(k).
 * https://oeis.org/A067827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 252, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: for n≥1, a(n) is the n-th even integer k such that
 *          (k/2) is non‑prime and σ(k+1) > σ(k),
 *          where σ(m) is the sum of the positive divisors of m.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;     /* array of computed terms */
    static size_t cap    = 0;       /* current capacity of the array   */
    static size_t count  = 0;       /* number of terms stored           */
    static idx_t last_checked = 2;  /* next even integer to evaluate   */

    /* a(1) is defined; if n<1 return 0 (undefined behaviour is fine) */
    if (n < 1)
        return (seq_t)0;

    /* initialise storage on first call */
    if (!terms) {
        cap = 64;
        terms = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!terms) return (seq_t)0; /* out of memory */
    }

    /* generate terms until we have at least n of them */
    while (count < (size_t)n) {
        /* look for a qualifying even integer starting from last_checked */
        for (idx_t k = last_checked; ; k += 2) {
            /* test k/2 for non‑primality (1 is allowed) */
            idx_t half = k >> 1;          /* k is even so k/2 is integer */
            if (half <= 1 || !is_prime(half)) {
                /* compute σ(k) and σ(k+1) */
                seq_t sig_k   = sigma_of(k);
                seq_t sig_k1  = sigma_of(k + 1);

                /* requirement σ(k+1) > σ(k) */
                if (sig_k1 > sig_k) {
                    /* store term */
                    if (count == cap) {
                        size_t new_cap = cap * 2;
                        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                        if (!tmp) return (seq_t)0; /* out of memory */
                        terms = tmp;
                        cap = new_cap;
                    }
                    terms[count++] = (seq_t)k;
                }
            }
            /* after checking this k, stop when we have enough terms */
            if (count >= (size_t)n) {
                last_checked = k + 2;   /* next candidate for future calls */
                break;
            }
        }
    }

    /* return the requested term (1‑based index) */
    return terms[n - 1];
}
