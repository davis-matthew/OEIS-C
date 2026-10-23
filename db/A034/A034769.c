/*
 * A034769  Dirichlet convolution of Catalan numbers with Bell numbers.
 * https://oeis.org/A034769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} Catalan(d-1) * Bell(n/d-1) (n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/catalan.h"
#include "../utils/binom.h"

static seq_t *bell_cache = NULL;   /* B[0] .. B[n] (B[0] = 1)   */
static size_t bell_cap = 0;        /* allocated size of bell_cache */

/* Fill bell_cache up to index n (inclusive).  B(0) and B(1) are set.   */
static void ensure_bell_cache(idx_t n)
{
    if (bell_cache && (idx_t)bell_cap > n)
        return;                   /* already large enough */

    size_t needed = (size_t)n + 1; /* indices 0 .. n */
    seq_t *arr;

    if (bell_cache) {
        arr = (seq_t *)realloc(bell_cache, needed * sizeof(seq_t));
        if (!arr) return;            /* allocation failure */
        for (size_t i = bell_cap; i < needed; ++i) arr[i] = 0;
    } else {
        arr = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!arr) return;
        for (size_t i = 0; i < needed; ++i) arr[i] = 0;
    }

    idx_t start = 0;
    if (!bell_cache) {              /* first run: initialise B0, B1 */
        arr[0] = 1;                 /* B0 = 1 */
        if (needed > 1) arr[1] = 1; /* B1 = 1 */
        start = 2;
    } else {
        start = (idx_t)bell_cap;
    }

    /* Compute Bell numbers B(m) for m = start .. n using the recurrence
       B(m) = Σ_{k=0}^{m-1} B(k) * C(m-1, k).                          */
    for (idx_t m = start; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            sum += arr[k] * binom((idx_t)m - 1, k);
        }
        arr[m] = sum;
    }

    bell_cache = arr;
    bell_cap = needed;
}

/* Dirichlet convolution of Catalan numbers with Bell numbers   */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* convention for n < 1 */

    ensure_bell_cache(n);             /* B[0..n] are ready */

    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            seq_t term1 = catalan(d - 1) * bell_cache[n / d - 1];
            result += term1;

            /* second divisor n/d when distinct */
            if (d * d != n) {
                idx_t d2 = n / d;
                seq_t term2 = catalan(d2 - 1) * bell_cache[d - 1];
                result += term2;
            }
        }
    }
    return result;
}
