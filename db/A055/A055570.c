/*
 * A055570  Sum of digits of (a(n)^4) is greater than or equal to a(n).
 * https://oeis.org/A055570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the nth nonnegative integer a such that the sum of the
 *            decimal digits of a^4 is greater than or equal to a. Computed by
 *            testing successive integers and caching the results in an array.
 *            O(n) time, O(n) memory
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms = NULL;      /* cached sequence values                */
static size_t terms_len   = 0;    /* number of values currently stored     */
static size_t terms_cap   = 0;    /* capacity of the array                  */
static idx_t  next_candidate = 1; /* next integer to test (after the 0)     */

/* grow the dynamic array of cached terms */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t newcap = terms_cap ? terms_cap << 1 : 16;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    terms = tmp;
    terms_cap = newcap;
}

/* extend the cache until at least terms[n] is computed */
static void ensure_terms(idx_t n)
{
    if (terms_len > n) return;

    /* initialise with the first term a(0)=0 */
    if (terms_len == 0) {
        grow_terms(1);
        terms[0] = (seq_t)0u;
        terms_len = 1;
        next_candidate = 1;
    }

    while (terms_len <= n) {
        idx_t cand = next_candidate;
        /* compute cand^4 using 128‑bit arithmetic   */
        unsigned __int128 sq   = (unsigned __int128)cand * cand;
        unsigned __int128 pow4 = sq * sq;

        /* digit sum of pow4 */
        unsigned sum = 0;
        unsigned __int128 tmp = pow4;
        while (tmp) {
            sum += (unsigned)(tmp % 10);
            tmp /= 10;
        }

        /* accept if digit sum ≥ cand */
        if (sum >= (unsigned)cand) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)cand;
        }
        ++next_candidate;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return terms[n];
}
