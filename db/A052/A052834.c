/*
 * A052834  a(n) = Bell(n+1)-Bell(n)-1, n>0.
 * https://oeis.org/A052834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Bell(n+1)-Bell(n)-1 (OEIS A052834)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n).  Uses a Stirling‑of‑second‑kind recurrence
   to generate Bell numbers up to the needed index.  All
   arithmetic is performed in the type seq_t. */
seq_t a(idx_t n)
{
    /* Return the first term directly. */
    if (n == 0)
        return (seq_t)0;

    /* Static state: indices of the last computed Bell number
       and the corresponding arrays. */
    static seq_t *bell = NULL;            /* Bell[0 .. last_n]   */
    static seq_t *S    = NULL;            /* Stirling values for current n (size last_n+1) */
    static idx_t  last_n = 0;             /* highest n for which bell[n] is valid */

    /* We need Bell numbers up to n+1. */
    idx_t needed = n + 1;

    /* First call: allocate minimal arrays. */
    if (bell == NULL) {
        size_t cap = 1;                   /* only Bell(0) and S(0) are needed initially */
        bell = (seq_t *)malloc(cap * sizeof(seq_t));
        S    = (seq_t *)malloc(cap * sizeof(seq_t));
        if (bell == NULL || S == NULL) exit(1);
        bell[0] = (seq_t)1;              /* Bell(0) = 1 */
        S[0]    = (seq_t)1;              /* S(0,0) = 1 */
        last_n  = 0;
    }

    /* Grow the tables if a higher index is required. */
    if (needed > last_n) {
        /* New capacity for indices 0 .. needed. */
        size_t new_cap = (size_t)needed + 1;
        seq_t *new_bell = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        seq_t *new_S    = (seq_t *)realloc(S,    new_cap * sizeof(seq_t));
        if (new_bell == NULL || new_S == NULL) exit(1);
        bell = new_bell;
        S    = new_S;

        /* Initialise new slots to zero. */
        for (size_t i = last_n + 1; i < new_cap; ++i) {
            bell[i] = (seq_t)0;
            S[i]    = (seq_t)0;
        }

        /* Compute Bell numbers for all nn in (last_n, needed]. */
        for (idx_t nn = last_n + 1; nn <= needed; ++nn) {
            /* Update Stirling numbers for the new row nn.
               Use the recurrence S(nn,k) = k * S(nn‑1,k) + S(nn‑1,k‑1). */
            for (long long k = nn; k >= 1; --k) {
                S[k] = (seq_t)k * S[k] + S[(size_t)k - 1];
            }
            S[0] = (seq_t)0;              /* S(nn,0) = 0 for nn > 0 */

            /* Bell(nn) is the sum of the Stirling numbers of the row. */
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k <= nn; ++k) {
                sum += S[k];
            }
            bell[nn] = sum;
        }

        last_n = needed;
    }

    /* a(n) = Bell(n+1) – Bell(n) – 1  for n > 0 */
    return bell[n + 1] - bell[n] - (seq_t)1;
}
