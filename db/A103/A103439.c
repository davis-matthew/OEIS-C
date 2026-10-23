/*
 * A103439  a(n) = Sum_{i=0..n-1} Sum_{j=0..i} (i-j+1)^j.
 * https://oeis.org/A103439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{n-1} Σ_{j=0}^{i} (i-j+1)^j (OEIS A103439)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *seq_arr = NULL;  /* a(n) values */
static seq_t *pow_arr = NULL;  /* pow_arr[t] = t^(current_exponent)  (current_exponent = k-t) */
static idx_t  max_comp = 0;    /* largest n whose a(n) has been computed */

/* Compute the sequence value for an arbitrary non‑negative n */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 0; no allocation required. */
    if (n == 0) return 0;

    /* Reserve space up to index n (indices 0..n).  The arrays are
       resized only when a larger n is requested. */
    if (max_comp < n) {
        idx_t new_size = n + 1;           /* indices 0..n */

        pow_arr = (seq_t *)realloc(pow_arr, new_size * sizeof(seq_t));
        seq_arr = (seq_t *)realloc(seq_arr, new_size * sizeof(seq_t));

        /* Initialise new entries – they are zeroed by the realloc in
           most implementations, but we do it explicitly for
           portability. */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_arr[i] = 0;
            pow_arr[i] = 0;
        }

        /* The very first entry is a(0)=0.  It is already set to 0
           above, but we set it explicitly for clarity. */
        seq_arr[0] = 0;

        /* Compute missing terms from (max_comp+1) up to n. */
        for (idx_t k = max_comp + 1; k <= n; ++k) {
            /* sum_f = 1 (corresponding to t = k) + Σ_{t=1}^{k-1} t^(k-t) */
            seq_t sum_f = 1;
            for (idx_t t = 1; t < k; ++t) {
                sum_f += pow_arr[t];
            }

            /* a(k) = a(k-1) + sum_f */
            seq_arr[k] = seq_arr[k - 1] + sum_f;

            /* Update pow_arr for the next outer iteration:
               for each t < k, multiply the current power by t
               (exponent increases by 1).  Finally set pow_arr[k] = k^1. */
            for (idx_t t = 1; t < k; ++t) {
                pow_arr[t] *= t;
            }
            pow_arr[k] = k;      /* k^1 */
        }

        max_comp = n;
    }

    return seq_arr[n];
}
