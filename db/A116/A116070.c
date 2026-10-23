/*
 * A116070  The digits of pi(n)=A000720(n) are obtained by adding pairs of
 *          adjacent digits of n.
 * https://oeis.org/A116070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   compute_pi, match
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that π(n) equals the sum of adjacent digits of n
   (OEIS A116070).  The function returns the nth such number, with a(1)=15. */
/* Implementation note: each call searches forward from the last found
   value until the requested count is reached.  The prime‑counting function
   compute_pi is supplied by the build.  No additional external libraries
   are used beyond the standard C headers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"
#include "../utils/match.h"

/* --- state ---------------------------------------------------------- */
static idx_t      current_n   = 0;   /* last tested integer */
static size_t     found_cnt   = 0;   /* number of values stored in results */
static seq_t     *results     = NULL;   /* array of found numbers */
static size_t     results_cap = 0;      /* capacity of results */

/* --- helpers -------------------------------------------------------- */
static void ensure_capacity(size_t need)
{
    if (results_cap >= need) return;
    size_t new_cap = results_cap ? results_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    results = tmp;
    results_cap = new_cap;
}

/* --- main function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* outside defined range */

    ensure_capacity((size_t)n);

    while (found_cnt < (size_t)n) {
        ++current_n;

        /* compute π(current_n) with the provided utility */
        seq_t pi_val = compute_pi(current_n);

        if (match(current_n, pi_val))
            results[found_cnt++] = (seq_t)current_n;
    }

    return results[n - 1];
}
