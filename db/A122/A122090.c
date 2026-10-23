/*
 * A122090  a(1)=1. a(n) = smallest integer that is greater than a(n-1) and is
 *          coprime to the largest nonprime (1 or composite) occurring among
 *          the first (n-1) terms of the sequence.
 * https://oeis.org/A122090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) is the smallest integer greater than
 *            a(n-1) that is coprime to the largest non‑prime (1 or composite)
 *            among a(1)…a(n-1)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

/* Static storage for the sequence */
static seq_t *seq      = NULL;      /* array of computed terms        */
static size_t seq_cap  = 0;         /* allocated capacity             */
static size_t seq_len  = 0;         /* number of terms stored         */
static seq_t  max_nonprime = 1;     /* largest non‑prime seen so far  */

/* Ensure the array can hold at least 'need' elements.  Allocate or
   grow the underlying storage as necessary.  */

static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];          /* C++: constructs all elements */
#else
    new_arr = (seq_t*)malloc(new_cap * sizeof(*new_arr));
    if (!new_arr) abort();                /* out of memory */
#endif

    /* Copy existing terms */
    for (size_t i = 0; i < seq_len; ++i)
        new_arr[i] = seq[i];

    /* Release old storage */
    if (seq) {
#ifdef __cplusplus
        delete[] seq;
#else
        free(seq);
#endif
    }

    seq   = new_arr;
    seq_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                            /* undefined for n < 1 */

    /* Compute terms up to n if necessary */
    for (size_t i = seq_len; i < (size_t)n; ++i) {
        ensure_capacity(seq_len + 1);         /* guarantee room */

        if (i == 0) {                        /* a(1) = 1 */
            seq[i] = 1;
            max_nonprime = 1;
        } else {
            seq_t prev = seq[i - 1];
            seq_t cand = prev;
            ++cand;                           /* start with a(i-1)+1 */

            while (gcd(cand, max_nonprime) != 1) {
                ++cand;                       /* find next coprime */
            }

            seq[i] = cand;

            /* Update largest non‑prime if necessary */
            if ((cand > max_nonprime) && !is_prime_big(cand)) {
                max_nonprime = cand;
            }
        }
    }

    seq_len = (size_t)n;
    return seq[n - 1];
}
