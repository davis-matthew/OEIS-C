/*
 * A133226  Number of possible 2 X n arrangements of black and white squares
 *          that can form two consecutive rows in an n X n crossword puzzle.
 * https://oeis.org/A133226
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + a(n-4) + f(n) where f(n) =
 *            b(n)^2 - 2*b(n-1)^2 + b(n-2)^2 - b(n-3)^2 - b(n-4)^2 - 2*b(n-3)
 *            b(n) is sequence A130578 (rows of an n×n crossword) Initial
 *            values: a(3)=1, a(4)=9, a(5)=36, a(6)=98
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include needed headers for memory allocation */
#include <stdlib.h>

/* External types provided by the build: idx_t and seq_t */
seq_t a(idx_t n)
{
    /* Handle the offset values directly */
    if (n == (idx_t)3) return (seq_t)1;
    if (n == (idx_t)4) return (seq_t)9;
    if (n == (idx_t)5) return (seq_t)36;
    if (n == (idx_t)6) return (seq_t)98;

    /* Compute the b sequence up to n using a 4‑state automaton.
       dp0 = strings ending in 1
       dp1 = strings ending in a single 0
       dp2 = strings ending in two 0s
       dp3 = strings ending in a run of >=3 0s   */
    seq_t *b = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!b) return (seq_t)0;            /* silent failure if malloc fails */

    b[0] = (seq_t)0;                    /* unused, but for completeness */

    seq_t dp0 = (seq_t)1; /* empty string ends in 1 (no zeros yet) */
    seq_t dp1 = (seq_t)0;
    seq_t dp2 = (seq_t)0;
    seq_t dp3 = (seq_t)0;

    for (idx_t len = (idx_t)1; len <= n; ++len) {
        /* transitions for the next length */
        seq_t next0 = dp0 + dp3;      /* 1 after a 1 or after a long zero run */
        seq_t next1 = dp0;            /* a new single zero starts a run */
        seq_t next2 = dp1;            /* second zero of a run */
        seq_t next3 = dp2 + dp3;      /* third‑+ zero or keep a long run */

        dp0 = next0;
        dp1 = next1;
        dp2 = next2;
        dp3 = next3;

        /* b[len] = strings ending in a valid long run (state 0 or state 3) minus
           the all‑ones string counted in state 0.  The subtraction is safe
           because dp0+dp3 ≥ 1 for every positive length. */
        b[len] = dp0 + dp3 - (seq_t)1;
    }

    /* Compute a(n) iteratively from a(7) up to a(n) */
    seq_t a_prev4 = (seq_t)1;  /* a(3) */
    seq_t a_prev3 = (seq_t)9;  /* a(4) */
    seq_t a_prev2 = (seq_t)36; /* a(5) */
    seq_t a_prev1 = (seq_t)98; /* a(6) */
    seq_t a_curr  = (seq_t)0;  /* placeholder for a(i) */

    for (idx_t i = (idx_t)7; i <= n; ++i) {
        seq_t bn  = b[i];
        seq_t bn1 = b[i-1];
        seq_t bn2 = b[i-2];
        seq_t bn3 = b[i-3];
        seq_t bn4 = b[i-4];

        seq_t f_val = bn * bn;               /* b(n)^2 */
        f_val += bn2 * bn2;                  /* + b(n-2)^2 */
        f_val -= (seq_t)2 * bn1 * bn1;       /* - 2*b(n-1)^2 */
        f_val -= bn3 * bn3;                  /* - b(n-3)^2 */
        f_val -= bn4 * bn4;                  /* - b(n-4)^2 */
        f_val -= (seq_t)2 * bn3;             /* - 2*b(n-3)  */

        a_curr = (seq_t)2 * a_prev1 - a_prev2 + a_prev3 + a_prev4 + f_val;

        /* shift the window for the next iteration */
        a_prev4 = a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    free(b);
    return a_curr;
}
