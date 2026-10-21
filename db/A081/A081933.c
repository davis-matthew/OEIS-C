/*
 * A081933  a(1) = 1, a(n) is the smallest number coprime to n and beginning
 *          with a(n-1).
 * https://oeis.org/A081933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest integer > a(n‑1) whose decimal representation
 *            starts with a(n‑1) and which is coprime to n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow10, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow10.h"   /* pow10(idx_t e)  -> 10^e as seq_t      */
#include "../utils/seq_gcd.h" /* seq_gcd(seq_t a, seq_t b)           */

static seq_t *terms  = NULL; /* Array of computed terms (0‑based index)  */
static size_t terms_len = 0; /* Number of terms stored (i.e. terms_len >= 1 for a(1)) */
static size_t terms_cap = 0; /* Allocated capacity of terms array          */

/* Ensure that the terms array can hold at least `need` elements. */
static void
ensure_capacity(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap : 128;
    while (new_cap < need)
        new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Return the n-th term of A081933 (n >= 1).  Calls may be out of order;
   results are memoised. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but provide a value */

    /* Compute terms up to n if necessary. */
    if (terms_len < n) {
        ensure_capacity(n);
        for (idx_t i = terms_len + 1; i <= n; ++i) {
            if (i == 1) {            /* a(1) = 1 */
                terms[0] = (seq_t)1;
                continue;
            }

            seq_t prefix = terms[i - 2]; /* a(i‑1) */
            /* Search for the smallest number starting with 'prefix' */
            seq_t result = 0;
            for (idx_t k = 1; ; ++k) {
                seq_t powk   = pow10(k);                    /* 10^k   */
                seq_t base   = prefix * powk;               /* prefix * 10^k */
                seq_t upper  = base + powk - (seq_t)1;      /* largest in this block */
                seq_t cand   = base;

                /* Scan numbers in this block in increasing order. */
                while (1) {
                    if (seq_gcd(cand, (seq_t)i) == (seq_t)1) {
                        result = cand;
                        goto assign_result;
                    }
                    if (cand == upper)
                        break;    /* move to next block with larger k */
                    cand = cand + (seq_t)1;
                }
            }
assign_result:
            terms[i - 1] = result;
        }
        terms_len = n;
    }
    return terms[n - 1];
}
