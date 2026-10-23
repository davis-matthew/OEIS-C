/*
 * A162322  Take sequence A000005 (where A000005(n) = the number of divisors
 *          of n). To get {a(k)}, replace each run of multiple occurrences of
 *          the same integer in sequence A000005 with just one occurrence of
 *          that integer, such that a(n) never equals a(n+1).
 * https://oeis.org/A162322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the subsequence of τ(n) (number of divisors of n) *
 *            where consecutive equal values are collapsed to a single
 *            occurrence, * so that a(n) != a(n+1)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   tau_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/tau_simple.h"

/* Build‑time maximum number of distinct τ values that will be needed.
 * 100 000 covers all required n for the given limits (n ≤ 10 000). */
#define MAX_LEN 100000

static seq_t seq_buf[MAX_LEN];
static seq_t *seq = seq_buf;
static size_t seq_cap = MAX_LEN;
static size_t seq_len = 0;
static idx_t last_orig = 0;
static seq_t last_val;

/* Public sequence accessor. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for zero, harmless */

    /* Extend cached values until we have at least n terms. */
    while (seq_len < (size_t)n) {
        ++last_orig;                     /* next natural number */
        seq_t val = tau_simple(last_orig); /* τ(last_orig) */
        if (seq_len == 0 || val != last_val) {
            seq[seq_len++] = val;
            last_val = val;
        }
        /* otherwise skip duplicate */
    }
    return seq[n - 1];
}
