/*
 * A122749  Number of arrangements of n non-attacking bishops on an n X n
 *          board such that every square of the board is controlled by at
 *          least one bishop.
 * https://oeis.org/A122749
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (by Andy Huchala, Mar 22 2024):
 *  a(4*k)   = (k+1)^2 · ((2*k)!)^2
 *  a(4*k+1) = (1/3) · (k+2)·(1+4*k+6*k^2) · ((2*k)!)^2
 *  a(4*k+2) = 4 · (k+1)^4 · ((2*k)!)^2
 *  a(4*k+3) = (1/3) · (3+17*k+22*k^2+6*k^3) · (2*k)! · (2*k+2)! */
#include <stdlib.h>

/* Pre‑computed factorials up to 5002 (needed for all n ≤ 10000). */
#define MAX_FACT 5002
static seq_t *fact = NULL;

/* Initialise factorial table once. */
static void init_fact(void)
{
    if (fact != NULL)
        return;

    fact = (seq_t*)malloc((MAX_FACT + 1) * sizeof(seq_t));
    if (!fact) abort();               /* out of memory */

    fact[0] = (seq_t)1;                /* 0! = 1 */
    for (idx_t i = 1; i <= MAX_FACT; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)      /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    init_fact();                      /* ensure factorials are ready */

    idx_t k = n / 4;
    idx_t r = n % 4;
    seq_t fac  = fact[2*k];           /* (2*k)!   */

    switch (r) {
        case 0: {                    /* a(4*k)   */
            seq_t res = (seq_t)(k+1) * (seq_t)(k+1);  /* (k+1)^2 */
            res *= fac;
            res *= fac;
            return res;
        }
        case 1: {                    /* a(4*k+1) */
            seq_t num = (seq_t)(k+2) *
                         ( (seq_t)1 + (seq_t)4*k + (seq_t)6*k*k );
            /* (1/3)*num*((2*k)!)^2 */
            seq_t res = num * fac * fac / (seq_t)3;
            return res;
        }
        case 2: {                    /* a(4*k+2) */
            seq_t kp1 = (seq_t)(k+1);
            seq_t kp1_2 = kp1 * kp1;      /* (k+1)^2 */
            seq_t kp1_4 = kp1_2 * kp1_2;  /* (k+1)^4 */
            seq_t res = (seq_t)4 * kp1_4 * fac * fac;
            return res;
        }
        case 3: {                    /* a(4*k+3) */
            seq_t fac2 = fact[2*k + 2];   /* (2*k+2)!   */
            seq_t num  = (seq_t)3 +
                          (seq_t)17*k +
                          (seq_t)22*k*k +
                          (seq_t)6*k*k*k;
            /* (1/3)*num*(2*k)!*(2*k+2)! */
            seq_t res = num * fac * fac2 / (seq_t)3;
            return res;
        }
    }
    return (seq_t)0;   /* unreachable */
}
