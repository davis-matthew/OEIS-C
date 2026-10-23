/*
 * A155710  Intersection of A092572 and A154778: N = a^2 + 3b^2 = c^2 + 5d^2
 *          for some positive integers a,b,c,d.
 * https://oeis.org/A155710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A155710.
   The terms are the positive integers that can be written both as
   a^2 + 3b^2 and as c^2 + 5d^2 for positive integers a,b,c,d.
   The function generates the terms on demand and memoises them
   so subsequent calls are fast. */

/*   a(idx_t n)  returns the n-th term (a(1) is the first term). */

#include <stdint.h>
#include <stdlib.h>

/* Forward declarations for the types used by the surrounding build. */
typedef uint64_t idx_t;
/* seq_t is provided by the build system; we only use it as a value type. */

/* Memoisation storage. */
static seq_t *terms = NULL;        /* array of computed terms      */
static size_t terms_len = 0;       /* number of terms stored      */
static size_t terms_cap = 0;       /* current capacity of array   */

/* Ensure that at least 'n' terms have been generated and stored. */
static void ensure_terms(idx_t n)
{
    if (terms_len >= (size_t)n)
        return;                     /* already have enough          */

    /* Initial search range: generous enough for small n. */
    size_t limit = (size_t)(n * 20);
    if (limit < 1000)
        limit = 1000;

    while (1) {
        idx_t lim = (idx_t)limit;

        /* Allocate boolean flags for representability.       */
        uint8_t *rep1 = (uint8_t*)calloc(lim + 1, sizeof(uint8_t));
        uint8_t *rep2 = (uint8_t*)calloc(lim + 1, sizeof(uint8_t));
        if (!rep1 || !rep2) {
            free(rep1);
            free(rep2);
            exit(1);               /* allocation failure */
        }

        /* Numbers representable as a^2 + 3b^2 (positive a,b). */
        for (idx_t a = 1; a * a <= lim; ++a) {
            idx_t a2 = a * a;
            for (idx_t b = 1; ; ++b) {
                idx_t val = a2 + 3 * b * b;
                if (val > lim)
                    break;
                rep1[val] = 1;
            }
        }

        /* Numbers representable as c^2 + 5d^2 (positive c,d). */
        for (idx_t c = 1; c * c <= lim; ++c) {
            idx_t c2 = c * c;
            for (idx_t d = 1; ; ++d) {
                idx_t val = c2 + 5 * d * d;
                if (val > lim)
                    break;
                rep2[val] = 1;
            }
        }

        /* Free old term storage if we had a previous attempt. */
        free(terms);
        terms = NULL;
        terms_len = 0;
        terms_cap = 0;

        /* Build sorted list of intersection terms. */
        for (idx_t val = 1; val <= lim; ++val) {
            if (rep1[val] && rep2[val]) {
                if (terms_len == terms_cap) {
                    size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
                    seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(rep1);
                        free(rep2);
                        exit(1); /* allocation failure */
                    }
                    terms = tmp;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)val;
            }
        }

        free(rep1);
        free(rep2);

        if (terms_len >= (size_t)n)
            break;                  /* sufficient terms generated */

        /* Not enough terms yet – enlarge the search range. */
        limit *= 2;
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for n==0 – return 0 */

    ensure_terms(n);                   /* generate terms up to n   */
    return terms[n - 1];               /* a(1) is the first term   */
}
