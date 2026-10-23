/*
 * A100822  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n with k cells in the first column. (A deco polyomino is a
 *          directed column-convex polyomino in which the height, measured
 *          along the diagonal, is attained only in the last column).
 * https://oeis.org/A100822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle defined by T(n,k) =
 *            sum_{j=1..k}(n-j)! for k < n, T(n,n) = 1 (reading rows left to
 *            right)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t *fact = NULL;          /* factorial array */
static idx_t fact_cap = 0;          /* allocated capacity */
static idx_t fact_size = 0;         /* number of initialized entries */

/* Ensure factorial array contains values up to fact[need] (inclusive). */
static void ensure_factorial(idx_t need)
{
    if (need < fact_size)
        return;

    idx_t new_cap = fact_cap ? fact_cap << 1 : 1;
    while (new_cap <= need)
        new_cap <<= 1;

    seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!new_fact)
        exit(1);                      /* out of memory */

    fact = new_fact;
    fact_cap = new_cap;

    for (idx_t i = fact_size; i <= need; ++i) {
        if (i == 0)
            fact[i] = 1;
        else
            fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_size = need + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* out of range, but guard */

    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = (idx_t)((sqrt((double)8.0 * n + 1.0) - 1.0) / 2.0);
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Column position within the row. */
    idx_t prev = (r * (r - 1)) / 2;
    idx_t c = n - prev;                /* 1 <= c <= r */

    if (c == r)
        return 1;                      /* last element of the row */

    /* Ensure factorials up to (r-1)! are available. */
    ensure_factorial(r - 1);

    seq_t sum = 0;
    for (idx_t j = 1; j <= c; ++j) {
        sum += fact[r - j];            /* add (r-j)! */
    }
    return sum;
}
