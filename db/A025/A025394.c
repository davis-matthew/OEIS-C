/*
 * A025394  Numbers that are the sum of 4 distinct nonzero squares in 10 or
 *          more ways.
 * https://oeis.org/A025394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth element of OEIS A025394.
   The sequence consists of all integers that can be expressed
   as the sum of four distinct non‑zero squares in at least ten
   different ways.  For each call this routine expands a bound
   on the maximum square root used; it enumerates all quadruples
   (a<b<c<d) of distinct positive integers up to that bound,
   counts how many times each sum occurs, keeps only the sums
   with at least ten representations, and stores those sums
   in an internal sorted array.  The procedure repeats with a
   larger bound until it contains at least the requested term
   index.  The implementation uses only C standard library
   facilities and is fully compliant with C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *terms = NULL;        /* sorted list of sums with >=10 reps */
static size_t terms_len = 0;       /* number of stored terms */
static idx_t currentM = 0;         /* current maximum square root used */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 – not used */

    /* Ensure we have at least n terms stored. */
    while (terms_len < n) {
        /* Increase the search bound.  Start with 200, double each time. */
        if (currentM == 0)
            currentM = 200;
        else
            currentM *= 2;

        /* Pre‑compute square values for 1 .. currentM. */
        idx_t *squares = (idx_t *)malloc((size_t)(currentM + 1) * sizeof(idx_t));
        if (!squares) return (seq_t)0;  /* allocation failure */

        for (idx_t i = 1; i <= currentM; ++i)
            squares[i] = i * i;

        /* Maximum possible sum = 4 * currentM^2. */
        idx_t maxSum = 4 * currentM * currentM;

        /* Count occurrences of each sum. */
        uint32_t *cnt = (uint32_t *)calloc((size_t)(maxSum + 1), sizeof(uint32_t));
        if (!cnt) { free(squares); return (seq_t)0; }

        /* Enumerate all quadruples of distinct positive integers. */
        for (idx_t a = 1; a <= currentM - 3; ++a) {
            idx_t a2 = squares[a];
            for (idx_t b = a + 1; b <= currentM - 2; ++b) {
                idx_t ab = a2 + squares[b];
                for (idx_t c = b + 1; c <= currentM - 1; ++c) {
                    idx_t abc = ab + squares[c];
                    for (idx_t d = c + 1; d <= currentM; ++d) {
                        idx_t sum = abc + squares[d];
                        ++cnt[sum];
                    }
                }
            }
        }

        /* Gather all sums with at least 10 representations. */
        seq_t *tmp = (seq_t *)malloc((size_t)(maxSum + 1) * sizeof(seq_t));
        if (!tmp) { free(cnt); free(squares); return (seq_t)0; }

        size_t pos = 0;
        for (idx_t s = 0; s <= maxSum; ++s) {
            if (cnt[s] >= 10) {
                tmp[pos++] = (seq_t)s;
            }
        }

        /* Resize the array to the actual number of terms. */
        seq_t *newTerms = (seq_t *)realloc(tmp, pos * sizeof(seq_t));
        if (!newTerms) {
            free(tmp);
            free(cnt);
            free(squares);
            return (seq_t)0;
        }

        /* Replace old terms array. */
        free(terms);
        terms = newTerms;
        terms_len = pos;

        free(cnt);
        free(squares);
    }

    /* Return the (n‑1)th term (zero‑based index). */
    return terms[n - 1];
}
