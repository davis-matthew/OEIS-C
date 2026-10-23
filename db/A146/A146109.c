/*
 * A146109  Bell numbers (A000110) read mod 19.
 * https://oeis.org/A146109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n) mod 19 (Bell numbers, A000110)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ----- static state ----------------------------------------------------- */
static seq_t *bell = NULL;        /* bell[n] = B_n mod 19                 */
static size_t bell_cap = 0;       /* allocated capacity (in elements)     */
static idx_t  bell_size = 0;      /* number of entries already computed   */

static int *S_prev = NULL;        /* Stirling row for n-1, S_prev[k]      */
static int *S_curr = NULL;        /* Stirling row for n,   S_curr[k]      */
static size_t S_cap = 0;          /* allocated capacity for Stirling rows */

/* ----- helper to ensure capacity for the bell array --------------------- */
static void ensure_bell_cap(idx_t n)
{
    if (bell_cap <= (size_t)n) {
        size_t new_cap = ((size_t)n + 1) * 2;
        bell = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        bell_cap = new_cap;
    }
}

/* ----- helper to ensure capacity for the Stirling arrays ---------------- */
static void ensure_S_cap(idx_t n)
{
    /* need space for indices 0..n inclusive */
    if (S_cap <= (size_t)n) {
        size_t new_cap = ((size_t)n + 1) * 2;
        S_prev = (int *)realloc(S_prev, new_cap * sizeof(int));
        S_curr = (int *)realloc(S_curr, new_cap * sizeof(int));
        S_cap = new_cap;
    }
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ---- lazy initialisation ------------------------------------------ */
    if (bell_size == 0) {
        ensure_bell_cap(0);
        bell[0] = (seq_t)1;      /* B_0 = 1 */
        bell_size = 1;

        ensure_S_cap(0);
        S_prev[0] = 1;           /* Stirling row 0: S(0,0)=1 */
    }

    /* ---- make sure we have computed up to n ----------------------------- */
    if (bell_size <= n) {
        ensure_bell_cap(n);

        for (idx_t m = bell_size; m <= n; ++m) {
            /* Compute S(m, k) for k=1..m and sum them for B_m             */
            ensure_S_cap(m);

            S_curr[0] = 0;                    /* S(m,0)=0 for m>0 */
            int sum = 0;

            for (int k = 1; k <= (int)m; ++k) {
                int spk = (k < (int)m) ? S_prev[k] : 0; /* S(m-1,k)        */
                int prev = S_prev[k - 1];              /* S(m-1,k-1)      */
                int val = ( (k * spk) % 19 + prev ) % 19;
                S_curr[k] = val;
                sum += val;
            }

            bell[m] = (seq_t)(sum % 19);

            /* swap S_prev and S_curr for next iteration                 */
            int *tmp = S_prev;
            S_prev = S_curr;
            S_curr = tmp;
        }
        bell_size = n + 1;
    }

    return bell[n];
}
