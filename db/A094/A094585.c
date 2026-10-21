/*
 * A094585  Triangle T of all positive differences of distinct Fibonacci
 *          numbers; also, triangle of all sums of consecutive distinct
 *          Fibonacci numbers.
 * https://oeis.org/A094585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = fib(r+3) - fib(r+3-c) * where r is the row index such
 *            that r(r+1)/2 ≥ n, * and c = n - r(r-1)/2 is the column position
 *            in that row
 * Range:     u8 n <= 57, u16 n <= 235, u32 n <= 994, u64 n <= 4100,
 *            u128 n <= 9473, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n = 0, return 0 */

    /* Find the row r (1‑based) and column c (1‑based) for the index n. */
    idx_t r = 1;            /* current row */
    idx_t sum = 1;          /* cumulative count up to current row = r(r+1)/2 */
    while (sum < n) {
        ++r;
        sum += r;           /* now sum = r(r+1)/2 */
    }
    idx_t prev_sum = sum - r; /* cumulative up to previous row = r(r-1)/2 */
    idx_t c = n - prev_sum;   /* column in current row */

    /* Need Fibonacci numbers up to index r+3.  Build a static array of
       Fibonacci numbers that grows as required. */
    static seq_t *fib = NULL;          /* array of Fibonacci numbers */
    static idx_t fib_cap = 0;          /* allocated capacity (number of entries) */

    idx_t need = r + 3;                /* we need F(0) … F(r+3) */
    if (need >= fib_cap) {
        idx_t new_cap = need + 16;
        seq_t *tmp = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        fib = tmp;
        fib_cap = new_cap;
    }

    /* Compute Fibonacci numbers from scratch (simple iterative addition). */
    fib[0] = (seq_t)0;
    if (need >= 1) fib[1] = (seq_t)1;
    for (idx_t i = 2; i <= need; ++i) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    /* Compute the desired element of the triangle. */
    seq_t val = fib[r + 3] - fib[r + 3 - c];
    return val;
}
