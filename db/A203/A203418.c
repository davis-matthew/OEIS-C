/*
 * A203418  Vandermonde determinant of the first n composite numbers
 *          (A002808).
 * https://oeis.org/A203418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{1 <= i < j <= n} (x_j - x_i) where x_k is the
 *            k-th composite number (4, 6, 8, 9, ...). a(1) = 1 by convention
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------
   Small prime test (trial division).  Works for numbers up
   to at least 2^64, but only a few million tests are executed
   for n up to 10000, so this is adequate.
   ------------------------------------------------------------ */
static bool is_prime_simple(idx_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------
   Dynamic storage for composites and a(n) values.
   ------------------------------------------------------------ */
static idx_t *composites = NULL;
static size_t composites_cap = 0, composites_len = 0;
static idx_t next_to_test = 4;           /* next integer to test for compositeness */

static seq_t *a_vals = NULL;
static size_t a_cap = 0, a_len = 0;

/* Ensure composites array can hold at least 'need' elements. */
static void ensure_composite_cap(size_t need) {
    if (composites_cap >= need) return;
    size_t new_cap = composites_cap ? composites_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(composites, new_cap * sizeof(idx_t));
    composites = tmp;
    composites_cap = new_cap;
}

/* Ensure a_vals array can hold at least 'need' elements. */
static void ensure_a_cap(size_t need) {
    if (a_cap >= need) return;
    size_t new_cap = a_cap ? a_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    a_vals = tmp;
    a_cap = new_cap;
}

/* Generate composites until we have at least 'n' entries. */
static void generate_composites(idx_t n) {
    while (composites_len < (size_t)n) {
        idx_t m = next_to_test++;
        if (!is_prime_simple(m)) {
            ensure_composite_cap(composites_len + 1);
            composites[composites_len++] = m;
        }
    }
}

/* Compute a(n) values up to n if not already computed. */
static void compute_until(idx_t n) {
    if (a_len >= (size_t)n) return;             /* already computed */

    /* Ensure we have enough composites. */
    generate_composites(n);

    /* Compute new a values. */
    while (a_len < (size_t)n) {
        if (a_len == 0) {                       /* a(1) = 1 */
            ensure_a_cap(a_len + 1);
            a_vals[a_len++] = (seq_t)1;
            continue;
        }

        /* x_new is the next composite (index a_len). */
        idx_t x_new = composites[a_len];

        /* Product of differences with all previous composites. */
        seq_t prod = (seq_t)1;
        for (idx_t i = 0; i < a_len; ++i) {
            prod *= (seq_t)(x_new - composites[i]);
        }

        /* New a value. */
        seq_t new_val = a_vals[a_len - 1] * prod;

        ensure_a_cap(a_len + 1);
        a_vals[a_len++] = new_val;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;                 /* undefined for n < 1 */
    compute_until(n);
    return a_vals[n - 1];
}
