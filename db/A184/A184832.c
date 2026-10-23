/*
 * A184832  a(n) = smallest k such that A005117(n+1) = A005117(n) +
 *          (A005117(n) mod k), or 0 if no such k exists.
 * https://oeis.org/A184832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A184832 – Weight of squarefree numbers.
 *
 *  For n ≥ 1:
 *      let A = A005117(n),   B = A005117(n+1)
 *      Δ = B - A
 *      find the smallest k > Δ such that Δ = A mod k
 *  Return that k, or 0 if no such k exists.
 *
 *  The implementation caches the A005117 sequence (square‑free
 *  numbers).  It generates numbers on demand and performs a simple
 *  divisor search on the value A‑Δ.  This runs in O(n) per call for
 *  n up to about 10 000 and works for any seq_t/idx_t sizes supported
 *  by the build configuration.
 */

#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------------*/
/*  Test whether an unsigned integer (idx_t) is square‑free.  This   */
/*  works for numbers well below 2⁶⁴, which is sufficient for the   */
/*  required values of A005117(n) (≈ 1.65 n).  Returns true if no   */
/*  squared prime divides the argument.                             */
/*------------------------------------------------------------------*/
static bool is_squarefree_idx(idx_t m)
{
    if (m <= 1)
        return true;
    for (idx_t p = 2; p * p <= m; ++p)
        if (m % (p * p) == 0)
            return false;
    return true;
}

/*------------------------------------------------------------------*/
/*  Cached array of square‑free numbers (A005117).  Elements are    */
/*  stored 1‑based: sqf[0] = 1, sqf[1] = 2, …                        */
/*------------------------------------------------------------------*/
static seq_t *sqf      = NULL;   /* array of square‑free numbers      */
static idx_t  sqf_len  = 0;      /* number of elements stored         */
static idx_t  sqf_cap  = 0;      /* allocated capacity                */
static idx_t  next_val = 1;      /* next integer to test for sanity   */

/* Ensure we have at least 'need' elements in the cache.  The function */
static void ensure_sqf(idx_t need)
{
    if (sqf_len >= need)
        return;

    if (sqf_cap == 0) {
        sqf_cap = 32;
        sqf = (seq_t *)malloc(sqf_cap * sizeof(seq_t));
    }

    while (sqf_len < need) {
        /* Find next square‑free integer. */
        while (!is_squarefree_idx(next_val))
            ++next_val;

        /* Resize if necessary. */
        if (sqf_len == sqf_cap) {
            sqf_cap <<= 1;
            sqf = (seq_t *)realloc(sqf, sqf_cap * sizeof(seq_t));
        }

        /* Store the value and advance. */
        sqf[sqf_len++] = (seq_t)next_val;
        ++next_val;
    }
}

/*------------------------------------------------------------------*/
/*  Main sequence function.                                          */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined below offset */

    /* Need the n‑th and (n+1)‑th square‑free numbers. */
    ensure_sqf(n + 1);

    const seq_t A = sqf[n - 1];
    const seq_t B = sqf[n];
    const seq_t D = B - A;                /* Δ = B – A */

    /* If A ≤ 2Δ then no divisor of (A–Δ) can be > Δ. */
    if (A <= (seq_t)2 * D)
        return (seq_t)0;

    const seq_t T = A - D;                /* T = A – Δ = q·k */

    /* Convert to idx_t for divisor enumeration (T is small). */
    idx_t Tidx = (idx_t)T;
    idx_t Didx = (idx_t)D;

    seq_t best = (seq_t)0;

    /* Search all divisors > Δ. */
    for (idx_t d = 2; d * d <= Tidx; ++d) {
        if (Tidx % d == 0) {
            const seq_t k1 = (seq_t)d;
            const seq_t k2 = (seq_t)(Tidx / d);
            if (k1 > Didx && (best == (seq_t)0 || k1 < best))
                best = k1;
            if (k2 > Didx && (best == (seq_t)0 || k2 < best))
                best = k2;
        }
    }

    /* The divisor T itself might be the smallest > Δ. */
    if (best == (seq_t)0 && Tidx > Didx)
        best = T;

    return best;
}
