/*
 * A011965  Second differences of Bell numbers.
 * https://oeis.org/A011965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = B(n+2) - 2*B(n+1) + B(n) where B(k) is the k‑th Bell
 *            number (B(0)=1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches Bell numbers up to the highest n queried.
   Bell numbers are computed with the recurrence
     B(0) = 1
     B(m+1) = Σ_{k=0..m} C(m, k) * B(k)
   where C(m, k) is computed incrementally.  The algorithm is
   O(n^2) arithmetic operations and works for arbitrary-precision
   seq_t. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *bell = NULL;          /* array of stored Bell numbers, 0‑based */
static idx_t  max_bell = 0;         /* highest index computed so far */

/* ensure that bell[0..n] are defined */
static void ensure_bell(idx_t n)
{
    if (n <= max_bell) return;          /* already computed */

    /* Resize the array if needed */
    if (bell == NULL) {
        bell = (seq_t*)calloc(1, sizeof(seq_t));
        if (bell == NULL) abort();
        bell[0] = (seq_t)1;
        max_bell = 0;
    }

    /* Allocate space up to n */
    seq_t *tmp = (seq_t*)realloc(bell, (n + 1) * sizeof(seq_t));
    if (tmp == NULL) abort();
    bell = tmp;

    /* Compute missing Bell numbers */
    for (idx_t m = max_bell + 1; m <= n; ++m) {
        if (m == 0) {
            bell[0] = (seq_t)1;            /* B(0) = 1 */
            continue;
        }

        seq_t sum   = 0;                    /* Σ C(m-1, k) * B(k)  */
        seq_t binom = 1;                    /* C(m-1, 0)            */
        idx_t limit = m - 1;                /* number of terms in sum */

        for (idx_t k = 0; k <= limit; ++k) {
            sum += binom * bell[k];
            if (k < limit)                  /* update binomial to next */
                binom = binom * (limit - k) / (k + 1);
        }
        bell[m] = sum;                     /* B(m) computed */
    }

    max_bell = n;
}

seq_t a(idx_t n)
{
    /* We need B(n), B(n+1) and B(n+2) */
    ensure_bell(n + 2);

    seq_t res = bell[n + 2];
    res -= 2 * bell[n + 1];
    res += bell[n];
    return res;
}
