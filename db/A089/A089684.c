/*
 * A089684  Numbers k such that 2*phi(k) > k.
 * https://oeis.org/A089684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089684 : numbers k such that 2*phi(k) > k
 *
 *  The sequence consists of the positive integers k for which
 *  twos times Euler's totient of k is strictly greater than k.
 *  The function generates values lazily: each call expands a
 *  totient sieve until it has produced at least n qualifying
 *  numbers and then returns the (n‑1)‑th entry of that list.
 *  No hard‑coded terms are used; the algorithm works for any
 *  seq_t (8,16,32,64 or 128‑bit unsigned, or arbitrary precision)
 *  and any idx_t (uint64_t) provided by the build system.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ----------  Cached data  ---------- */
static seq_t  *phi       = NULL;    /* Euler's totient values   */
static size_t  phi_size  = 0;       /* allocated size of phi    */

static seq_t  *valid     = NULL;    /* list of k with 2φ(k) > k */
static size_t  valid_len = 0;       /* number of elements       */

static size_t  current_limit = 0;   /* range up to which φ is computed */

/* ----------  Helpers  ---------- */

/* Rebuild φ[1..limit] using a simple sieve of Eratosthenes. */
static void recompute_phi(size_t limit)
{
    /* Grow the array if necessary */
    if (phi_size <= limit) {
        seq_t *tmp = (seq_t *)realloc(phi, (limit + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        phi = tmp;
        phi_size = limit + 1;
    }

    /* Initialise φ[i] = i */
    phi[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        phi[i] = (seq_t)i;

    /* Apply the totient sieve */
    for (size_t i = 2; i <= limit; ++i) {
        if (phi[i] == (seq_t)i) {            /* i is prime */
            for (size_t j = i; j <= limit; j += i) {
                phi[j] -= phi[j] / (seq_t)i; /* avoid modulo overflow for small seq_t */
            }
        }
    }
}

/* Rebuild the list of qualifying k up to `limit`. */
static void rebuild_valid(size_t limit)
{
    size_t cnt = 0;
    for (size_t k = 1; k <= limit; ++k) {
        if ((idx_t)2 * (idx_t)phi[k] > (idx_t)k) ++cnt;
    }

    seq_t *tmp = (cnt ? (seq_t *)realloc(valid, cnt * sizeof(seq_t)) : NULL);
    if (!tmp && cnt) exit(EXIT_FAILURE);
    valid = tmp;
    valid_len = cnt;

    size_t pos = 0;
    for (size_t k = 1; k <= limit; ++k) {
        if ((idx_t)2 * (idx_t)phi[k] > (idx_t)k)
            valid[pos++] = (seq_t)k;
    }

    current_limit = limit;
}

/* Expand the cached data until we have at least `n` terms. */
static void ensure_for_n(idx_t n)
{
    if (n < 1) return;
    while (valid_len < (size_t)n) {
        size_t new_limit = current_limit ? current_limit * 2 : 1000;
        if (new_limit < (size_t)(n * 3))
            new_limit = (size_t)(n * 3);
        recompute_phi(new_limit);
        rebuild_valid(new_limit);
    }
}

/* ----------  Public function  ---------- */

seq_t a(idx_t n)
{
    ensure_for_n(n);
    return valid[n - 1];
}
