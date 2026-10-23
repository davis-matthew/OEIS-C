/*
 * A132068  Irregular array: row n has A000010(n) terms: the sum of the first
 *          m terms of row n is the m-th positive integer which is coprime to
 *          n.
 * https://oeis.org/A132068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) follows the definition of OEIS A132068. The irregular array
 *            is built row‑by‑row: row r has φ(r) terms. For the m‑th term of
 *            row r we take the m‑th positive integer coprime to r; the term
 *            equals the difference between successive such integers (first
 *            term is always 1). This implementation foll...
 * Range:     u8 none, u16 n <= 105, u32 n <= 105, u64 n <= 105, u128 n <= 105,
 *            big any n
 * Helpers:   calc_phi, plain_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"
#include "../utils/plain_gcd.h"

static idx_t *prefix = NULL;          /* cumulative φ(1..i) */
static idx_t  max_row = 0;            /* highest row pre‑computed */

/* Ensure that prefix[0..up_to] is filled with sums of φ(1..i). */
static void extend_rows(idx_t up_to)
{
    if (up_to <= max_row)
        return;

    idx_t old = max_row;
    max_row = up_to;

    if (prefix == NULL) {
        prefix = (idx_t *)calloc(max_row + 1, sizeof(idx_t));
        if (!prefix)
            abort();
    } else {
        idx_t *tmp = (idx_t *)realloc(prefix, (max_row + 1) * sizeof(idx_t));
        if (!tmp)
            abort();
        prefix = tmp;
    }

    idx_t cumulative = (old == 0) ? 0 : prefix[old];
    for (idx_t r = old + 1; r <= max_row; ++r) {
        seq_t phi_val = calc_phi((seq_t)r);   /* φ(r) */
        cumulative += (idx_t)phi_val;
        prefix[r] = cumulative;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */

    /* Extend precomputation until the linear index n is covered. */
    while (prefix == NULL || prefix[max_row] < n)
        extend_rows(max_row + 1024);

    /* Find the row that contains index n by binary search. */
    idx_t lo = 1, hi = max_row;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (prefix[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo;

    /* Position m of the term within the row. */
    idx_t row_start = (row == 1) ? 1 : prefix[row - 1] + 1;
    idx_t m = n - row_start + 1;

    /* First term is always 1. */
    if (m == 1)
        return (seq_t)1;

    /* Locate the m‑th coprime integer to 'row' and return the difference
       with the previous coprime. */
    idx_t count = 1;          /* we already have the first coprime: 1 */
    idx_t prev = 1;
    for (idx_t x = 2; x <= row; ++x) {
        if (plain_gcd(row, x) == 1) {
            ++count;
            if (count == m)
                return (seq_t)(x - prev);
            prev = x;
        }
    }

    /* Should never be reached. */
    return (seq_t)0;
}
