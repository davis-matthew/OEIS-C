/*
 * A215726  Numbers k such that the k-th triangular number is squarefree.
 * https://oeis.org/A215726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest index k such that the k‑th triangular number
 *            T_k = k*(k+1)/2 is squarefree. The sequence is the list of such
 *            k: a(1)=1, a(2)=2, a(3)=3, …
 * Range:     u8 n <= 15, u16 n <= 555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

/* Maximum number of terms we expect to store.  The sequence has
   density ≈0.48, so for n≈10000 the corresponding k is below
   21000.  20000 is therefore a safe limit for all tests. */
#define MAX_TERMS 20000

/* Static storage for memoised terms and progress state. */
static seq_t  g_terms[MAX_TERMS];      /* g_terms[i] holds a(i+1) */
static size_t g_len   = 0;             /* number of terms currently stored */
static idx_t  g_lastk = 0;             /* largest k already examined */

/* Return the n‑th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset 0, but safe */

    /* Ensure we have at least n terms computed. */
    while (g_len < n) {
        /* Test next k. */
        idx_t  k   = g_lastk + 1;
        seq_t  tri = (seq_t)k * ((seq_t)k + 1) / 2;

        if (is_squarefree(tri)) {
            if (g_len < MAX_TERMS) {
                g_terms[g_len++] = k;
            } else {
                /* This should not happen for the intended n range. */
                return 0;
            }
        }

        g_lastk = k;
    }

    /* Return the stored term.  g_terms[0] corresponds to a(1). */
    return g_terms[n - 1];
}
