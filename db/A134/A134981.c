/*
 * A134981  a(n) = A000110(n)*A000166(n).
 * https://oeis.org/A134981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000110(n) * A000166(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *bell = NULL;
static seq_t *der  = NULL;
static idx_t  cap  = 0;

/* Expand the cache to hold results up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cap) return;                /* already have enough */

    /* Reallocate larger arrays.  Cast to seq_t* for C++ compatibility. */
    seq_t *new_bell = (seq_t *)realloc(bell, (size_t)(n + 1) * sizeof(seq_t));
    seq_t *new_der  = (seq_t *)realloc(der,  (size_t)(n + 1) * sizeof(seq_t));
    if (!new_bell || !new_der) {
        /* Allocation failure – abort the program. */
        exit(1);
    }
    bell = new_bell;
    der  = new_der;

    /* Compute missing entries. */
    for (idx_t i = cap; i <= n; ++i) {
        if (i == 0) {          /* base cases */
            bell[0] = (seq_t)1;
            der[0]  = (seq_t)1;
            continue;
        }
        if (i == 1) {
            bell[1] = (seq_t)1;
            der[1]  = (seq_t)0;
            continue;
        }

        /* Derangement recurrence: D(i) = (i-1)*(D(i-1)+D(i-2)) */
        der[i] = ((seq_t)(i - 1)) * (der[i - 1] + der[i - 2]);

        /* Bell number recurrence: B(i) = Σ_{k=0}^{i-1} C(i-1,k) * B(k) */
        seq_t sum   = 0;
        seq_t binom = 1;        /* C(i-1,0) */
        idx_t kmax  = i - 1;
        for (idx_t k = 0; k <= kmax; ++k) {
            sum += binom * bell[k];
            if (k < kmax)
                binom = (binom * (seq_t)(kmax - k)) / (seq_t)(k + 1);
        }
        bell[i] = sum;
    }

    cap = n + 1;
}

/* Required public function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return bell[n] * der[n];
}
