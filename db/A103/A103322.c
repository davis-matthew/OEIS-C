/*
 * A103322  Expansion of 1 / ((1-x-x^2-x^3)*(1-x^2-x^3)).
 * https://oeis.org/A103322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - 2*a(n-4) - 2*a(n-5) - a(n-6)
 *            with initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=6, a(4)=11,
 *            a(5)=22
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *vals = NULL;   /* cached values */
static idx_t vals_size = 0;  /* number of elements currently cached */

/* Ensure that the values up to index n are computed and cached. */
static void ensure_vals(idx_t n)
{
    if (n < vals_size) return;                    /* already computed */

    idx_t new_size = n + 1;                       /* number of elements needed */
    if (vals == NULL)                            /* first allocation */
        vals = (seq_t *)malloc(new_size * sizeof(seq_t));
    else
        vals = (seq_t *)realloc(vals, new_size * sizeof(seq_t));

    if (vals == NULL) /* out of memory */
        exit(EXIT_FAILURE);

    /* Compute missing values */
    for (idx_t i = vals_size; i <= n; ++i) {
        if (i < 6) {                             /* initial conditions */
            switch (i) {
                case 0: vals[i] = (seq_t)1; break;
                case 1: vals[i] = (seq_t)1; break;
                case 2: vals[i] = (seq_t)3; break;
                case 3: vals[i] = (seq_t)6; break;
                case 4: vals[i] = (seq_t)11; break;
                case 5: vals[i] = (seq_t)22; break;
            }
        } else {                                 /* recurrence */
            seq_t v = vals[i-1];
            v += (seq_t)2 * vals[i-2];
            v += vals[i-3];
            v -= (seq_t)2 * vals[i-4];
            v -= (seq_t)2 * vals[i-5];
            v -= vals[i-6];
            vals[i] = v;
        }
    }
    vals_size = new_size;
}

/* Public entry point */
seq_t a(idx_t n)
{
    ensure_vals(n);
    return vals[n];
}
