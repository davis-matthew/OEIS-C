/*
 * A148689  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1, 1,
 *          0), (1, 0, -1), (1, 0, 1)}.
 * https://oeis.org/A148689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σ_{d=0..⌊n/2⌋} [ (n‑2d+1)/(n‑d+1) * C(n, n‑d) ] *
 *            C(d,⌊d/2⌋) * C(n‑d,⌊(n‑d)/2⌋)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* -------------- Central binomial coefficients C(k,⌊k/2⌋) ---------------- */

static seq_t *central_binoms = NULL;      /* central_binoms[k] = C(k,⌊k/2⌋) */
static idx_t  central_cap   = 0;          /* current allocated size */

/* Ensure central_binoms contains values up to n (inclusive).  */
static void
ensure_central(idx_t n)
{
    if (central_cap > n)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        return;   /* allocation failure – let the caller fail */

    /* copy old values if any */
    if (central_binoms) {
        for (size_t i = 0; i < (size_t)central_cap; ++i)
            new_arr[i] = central_binoms[i];
        free(central_binoms);
    }

    /* compute missing entries  */
    for (idx_t k = central_cap; k <= n; ++k) {
        if (k == 0)
            new_arr[0] = (seq_t)1;
        else if (k & 1u) {                      /* k = 2m+1  */
            idx_t m   = k / 2;
            new_arr[k] = new_arr[k-1] * (seq_t)k / (seq_t)(m + 1);
        } else {                                /* k = 2m    */
            /* C(2m, m) = C(2m‑1, m‑1) * 2  */
            new_arr[k] = new_arr[k-1] * (seq_t)2;
        }
    }

    central_binoms = new_arr;
    central_cap    = (idx_t)new_cap;
}

/* -------------- Main sequence function ----------------------------------- */

seq_t
a(idx_t n)
{
    /* Pre‑compute central binomial coefficients up to n. */
    ensure_central(n);

    /* Variables for the running binomial C(n, u), where u decreases. */
    idx_t u = n;                      /* current number of up (x+1) steps  */
    seq_t Cnu = (seq_t)1;             /* C(n, u) current value             */

    idx_t max_d = n >> 1;             /* floor(n/2)                         */
    seq_t sum   = (seq_t)0;

    for (idx_t d = 0; d <= max_d; ++d) {
        /* Current h = u - d = n - 2d                              */
        idx_t h = n - (d << 1);

        /* Ballot term for the x‑walk:  (h+1)/(u+1) * Cnu           */
        seq_t Bx = Cnu * (seq_t)(h + 1) / (seq_t)(u + 1);

        /* y‑walk (down steps)  and  z‑walk (up steps)             */
        seq_t term = Bx;
        term *= central_binoms[d];       /* C(d,⌊d/2⌋)    */
        term *= central_binoms[u];       /* C(u,⌊u/2⌋)    */
        sum  += term;

        /* Prepare for next iteration: decrement u and update Cnu    */
        if (d < max_d) {
            /* C(n, u‑1) = C(n, u) * u / (n - u + 1)                 */
            idx_t old_u = u;
            u = u - 1;
            Cnu = Cnu * (seq_t)old_u / (seq_t)(n - old_u + 1);
        }
    }

    return sum;
}
