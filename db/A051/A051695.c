/*
 * A051695  Number of degree-n even permutations of order exactly 4.
 * https://oeis.org/A051695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (A001473(n) + A051685(n))/2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = even permutations of S_n with cycle lengths 1,2,4
 *            that contain at least one 4‑cycle                            */

#include <stdint.h>
#include <stdbool.h>

/* maximum n that may be requested (≈10000) */
#define MAX_N 10000

/* tables of counts (global, zero‑initialised) */
static seq_t even_all[MAX_N + 1];
static seq_t odd_all[MAX_N + 1];
static seq_t even_no4[MAX_N + 1];
static seq_t odd_no4[MAX_N + 1];

/* highest n that has been computed */
static idx_t max_computed = 0;

/* Ensure that the tables contain values up to and including n. */
static void compute_to(idx_t n)
{
    if (n <= max_computed)
        return;

    /* initialise the base case (n = 0) if this is the first call */
    if (max_computed == 0) {
        even_all[0]  = 1;  /* empty permutation is even */
        odd_all[0]   = 0;
        even_no4[0]  = 1;
        odd_no4[0]   = 0;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* 1‑cycles, 2‑cycles and 4‑cycles (allow all of them) */
        even_all[i]  = even_all[i-1];
        odd_all[i]   = odd_all[i-1];

        if (i >= 2) {
            seq_t m = (seq_t)(i-1);                 /* (i‑1) ways to Pick the partner of the 2‑cycle */
            even_all[i] += m * odd_all[i-2];
            odd_all[i]  += m * even_all[i-2];
        }

        if (i >= 4) {
            seq_t m = (seq_t)(i-1) * (i-2) * (i-3); /* (i‑1)(i‑2)(i‑3) ways to pick the other 3 elements of a 4‑cycle */
            even_all[i] += m * odd_all[i-4];
            odd_all[i]  += m * even_all[i-4];
        }

        /* 1‑cycles and 2‑cycles only (no 4‑cycles) */
        even_no4[i] = even_no4[i-1];
        odd_no4[i]  = odd_no4[i-1];

        if (i >= 2) {
            seq_t m = (seq_t)(i-1);
            even_no4[i] += m * odd_no4[i-2];
            odd_no4[i]  += m * even_no4[i-2];
        }
    }

    max_computed = n;
}

/* Public entry point required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined for n ≥ 1 */
    compute_to(n);
    /* even permutations with a 4‑cycle = all even permutations − even permutations without 4‑cycles */
    return even_all[n] - even_no4[n];
}
