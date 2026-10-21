/*
 * A075305  Smallest prime not occurring earlier == 1 mod n-th composite
 *          number.
 * https://oeis.org/A075305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that * p ≡ 1 (mod c_n) and p
 *            has not been seen * earlier, where c_n is the n‑th composite
 *            number * (offset 1). Each a(n) is distinct
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   isPrime, term_contains
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"   /* bool isPrime(idx_t x); */
#include "../utils/term_contains.h"
 
/* seq_t and idx_t are supplied by the build system. */
 
/* Forward declaration of a helper to avoid a global static array. */
static bool term_contains(const seq_t *arr, size_t len, seq_t v);

/*--------------------------------------------------------------------*/
/*  Compute the n‑th term of A075305.  n ≥ 1.                        */
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /*---  static caches  -------------------------------------------------*/
    static seq_t *terms   = NULL;  /* terms[0] = a(1), …            */
    static size_t term_len = 0;    /* number of terms stored         */

    static seq_t *comps   = NULL;  /* composite numbers              */
    static size_t comp_len = 0;    /* number of composite numbers    */
    static idx_t next_int  = 4;     /* next integer to test for prime */

    /*---  ensure we have the first 'n' composite numbers  ----------------*/
    if (comp_len < n) {
        /* grow the composite array as needed */
        while (comp_len < n) {
            /* test whether 'next_int' is composite */
            if (!isPrime(next_int)) {
                comps = (seq_t *)realloc(comps,
                                         (comp_len + 1) * sizeof(seq_t));
                if (!comps) exit(EXIT_FAILURE);
                comps[comp_len++] = next_int;
            }
            ++next_int;
        }
    }

    /*---  compute missing terms up to index n  -----------------------------*/
    if (term_len < n) {
        /* grow the term array simply one by one. */
        for (size_t i = term_len; i < (size_t)n; ++i) {
            idx_t m = (idx_t)comps[i];          /* the i‑th composite   */
            idx_t cand = m + 1;                 /* first number ≡ 1 (mod m) */

            while (true) {
                if (isPrime(cand) &&
                    !term_contains(terms, term_len, (seq_t)cand)) {

                    /* store the new term */
                    terms = (seq_t *)realloc(terms,
                                             (term_len + 1) * sizeof(seq_t));
                    if (!terms) exit(EXIT_FAILURE);
                    terms[term_len++] = (seq_t)cand;
                    break;
                }
                cand += m;                       /* next candidate 1 (mod m) */
            }
        }
    }

    return terms[n - 1];
}

/*--------------------------------------------------------------------*/
/*  Test whether 'v' already appears in the first 'len' elements of   */
/*  the array 'arr'.  The helper is static and small.                  */
