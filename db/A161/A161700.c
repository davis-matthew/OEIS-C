/*
 * A161700  a(n) is the sum of the elements on the antidiagonal of the
 *          difference table of the divisors of n.
 * https://oeis.org/A161700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the sum of the elements on the antidiagonal of the *
 *            difference table of the divisors of n. For the divisors
 *            d(1)<…<d(k), * let Δ^0(d) = d, Δ^i+1(d)(j) =
 *            Δ^i(d)(j+1)-Δ^i(d)(j) * (i.e. successive finite differences).
 *            Then a(n) * = Σ_{i=0}^{k-1} Δ^i(d)(k-i-1). * This implementation
 *            generate...
 * Range:     i8 n <= 29, i16 n <= 179, i32 n <= 1259, i64 n <= 7559,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
 * Helper: integer floor square root.
 * No math.h or floating point is used; the loop finishes quickly
 * for n up to 10^12 (and even larger for 64‑bit).  The function
 * returns the largest s.t. (root+1)^2 > n.
 * ---------------------------------------------------------------------- */
static idx_t isqrt_u64(idx_t n)
{
    /* Simple Newton iteration: start with an overestimate.
       The product root*root fits into 128 bits for 64‑bit inputs,
       but we use a naïve loop that is fast for n <= 10^12.    */
    idx_t r = (idx_t)1;
    while ((idx_t)(r + 1) * (r + 1) <= n)
        r++;
    return r;
}

/* ----------------------------------------------------------------------
 * Helper: insertion sort for small arrays of seq_t.
 * -------------------------------------------------------------- */
static void sort_seq_t(seq_t *a, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        seq_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

/* ----------------------------------------------------------------------
 * Main sequence function.
 * -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Safe guard for very small n (offset is 1). */
    if (n == 0)
        return 0;

    /* Step 1: Collect all divisors of n. */
    idx_t limit = isqrt_u64(n);
    /* Allocate at most 2*limit + 2 elements for safety. */
    size_t max_div = (size_t)(2 * limit + 2);
    seq_t *divs = (seq_t *)malloc(max_div * sizeof(seq_t));
    if (!divs)
        return 0;  /* allocation failure */

    size_t cnt = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            divs[cnt++] = (seq_t)i;
            if (i != n / i) {
                divs[cnt++] = (seq_t)(n / i);
            }
        }
    }

    /* Step 2: Sort divisors ascending. */
    sort_seq_t(divs, cnt);

    /* Step 3: Compute antidiagonal sum. */
    seq_t sum = divs[cnt - 1];
    for (size_t step = 0; step + 1 < cnt; ++step) {
        /* Compute next difference row in place. */
        for (size_t i = 0; i + 1 < cnt - step; ++i) {
            divs[i] = divs[i + 1] - divs[i];
        }
        sum += divs[cnt - step - 2];
    }

    free(divs);
    return sum;
}
