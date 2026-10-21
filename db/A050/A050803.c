/*
 * A050803  Cubes expressible as the sum of two nonzero squares in at least
 *          one way.
 * https://oeis.org/A050803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = (k_n)^3 where k_n is the n‑th positive integer that can
   be written as a^2 + b^2 with a,b ≥ 1.  The sequence has offset 1,
   so a(1) = 8 (=2³).  The implementation builds a list of all such
   integers up to the required bound using a boolean sieve and
   dynamically enlarges the bound as more terms are requested. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for the cube roots (elements of A000404). */
static seq_t   *roots      = NULL;   /* array of roots, sorted ascending */
static size_t   roots_len  = 0;      /* number of roots stored */
static size_t   roots_cap  = 0;      /* allocated capacity for roots array */
static uint64_t limit      = 1;      /* current upper bound for sums of squares */

/* Generate all distinct sums of two squares ≤ 'lim' and store them in
   the global 'roots' array.  The array is rebuilt from scratch. */
static void generate_upto(uint64_t lim)
{
    /* Boolean marker array: 1 means the number can be expressed. */
    bool *m = (bool*)calloc(lim + 1, sizeof(bool));
    if (!m) abort();

    /* Mark all sums i^2 + j^2 (with i,j ≥ 1).  We only need j ≥ i to
       avoid duplicates. */
    for (uint64_t i = 1; i * i <= lim; ++i) {
        uint64_t ii = i * i;
        for (uint64_t j = i; ii + j * j <= lim; ++j) {
            m[ii + j * j] = true;
        }
    }

    /* Count how many numbers are marked to allocate the results array. */
    size_t cnt = 0;
    for (uint64_t s = 1; s <= lim; ++s) {
        if (m[s]) ++cnt;
    }

    /* Rebuild roots array.  Free previous contents if any. */
    free(roots);
    roots   = (seq_t*)malloc(cnt * sizeof(seq_t));
    if (!roots) abort();
    roots_len = 0;
    roots_cap = cnt;

    for (uint64_t s = 1; s <= lim; ++s) {
        if (m[s]) {
            roots[roots_len++] = (seq_t)s;
        }
    }

    free(m);
}

/* Ensure that at least 'n' roots are available.  If not, increase the
   search limit (doubling it each time) and regenerate the roots. */
static void ensure_terms(size_t n)
{
    while (roots_len < n) {
        limit <<= 1;                 /* double the search bound */
        generate_upto(limit);
    }
}

/* Public sequence function.  The sequence has offset 1.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n == 0, safe default */

    ensure_terms((size_t)n);       /* guarantee we have the n-th root */

    seq_t r = roots[n - 1];        /* n-th root (1‑based) */
    /* Compute r^3.  The expression is evaluated left‑to‑right. */
    seq_t cube = r * r;
    cube = cube * r;
    return cube;
}
