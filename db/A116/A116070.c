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
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-03)
 * Helpers:   mem, prime_pi
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
#include <stddef.h>
#include "../utils/prime_pi.h"
#include "../utils/mem.h"

/* Check whether the integer n satisfies the property with the given π(n). */
static bool match(idx_t n, seq_t pi_val)
{
    /* extract decimal digits (least significant first) */
    unsigned char digs[20];
    int len = 0;
    idx_t t = n;
    while (t) {
        digs[len++] = (unsigned char)(t % 10);
        t /= 10;
    }
    if (len < 2) return false;           /* need at least two digits */

    /* build the number formed by pairwise sums, most significant pair first */
    seq_t sum = 0;
    for (int i = len - 1; i > 0; --i) {
        int s = digs[i] + digs[i - 1];
        if (s > 9) return false;          /* must stay a single decimal digit */
        sum = sum * 10 + (seq_t)s;
    }
    return sum == pi_val;
}

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
    seq_t *tmp = mem_resize(seq_t, results, new_cap);
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
        seq_t pi_val = prime_pi(current_n);

        if (match(current_n, pi_val))
            results[found_cnt++] = (seq_t)current_n;
    }

    return results[n - 1];
}
