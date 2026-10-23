/*
 * A161906  Triangle read by rows in which row n lists the divisors of n that
 *          are <= sqrt(n).
 * https://oeis.org/A161906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the nth element of the triangle formed by the inferior
 *            divisors of each integer; row r lists all divisors d of r with d
 *            ≤ √r. The sequence is the concatenation of these rows (offset 1)
 * Range:     u8 n <= 199, u16 n <= 3550, u32 n <= 3550, u64 n <= 3550,
 *            u128 n <= 3550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static tables for divisor counts and prefix sums of inferior divisor
   counts.  For arbitrary-precision builds the tables are indexed by
   idx_t but hold values of type seq_t. */
static seq_t *divcnt = NULL;   /* divisor counts for each integer */
static seq_t *prefix = NULL;   /* prefix sums of (divcnt[i]+1)/2 */
static idx_t  current_size = 0; /* maximum index stored (>=1 if ready) */

/* Integer square root of an unsigned integer (idx_t).  Uses a simple
   binary search that is safe for arbitrarily large idx_t values. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            lo = 1;
            continue;
        }
        if (mid > n / mid) {          /* mid*mid > n without overflow */
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

/* Ensure the divisor and prefix tables extend at least up to index
   `needed` (inclusive).  Tables are grown to the next power of two
   for efficiency. */
static void ensure_tables(idx_t needed)
{
    if (needed <= current_size)
        return; /* already large enough */

    idx_t new_size = current_size ? current_size : 1;
    while (new_size <= needed)
        new_size <<= 1;           /* grow to next power of two */

    /* Allocate +1 extra slot so that indices 0..new_size are valid */
    divcnt   = (seq_t *)realloc(divcnt, (new_size + 1) * sizeof(seq_t));
    prefix   = (seq_t *)realloc(prefix, (new_size + 1) * sizeof(seq_t));

    /* Recompute divisor counts and prefix sums from scratch */
    for (idx_t i = 0; i <= new_size; ++i)
        divcnt[i] = 0;

    for (idx_t i = 1; i <= new_size; ++i)
        for (idx_t j = i; j <= new_size; j += i)
            divcnt[j] += (seq_t)1;

    prefix[0] = 0;
    for (idx_t i = 1; i <= new_size; ++i) {
        seq_t c = (divcnt[i] + (seq_t)1) / (seq_t)2; /* inferior divisors of i */
        prefix[i] = prefix[i - 1] + c;
    }

    current_size = new_size;
}

/* Main sequence function.  It is safe to call in any order and for
   any n ≥ 1.  For n == 0 a safe sentinel value (0) is returned. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for 0, return sentinel */

    /* Make tables big enough to handle index n */
    ensure_tables(n);

    /* Binary search for the smallest row r with prefix[r] ≥ n. */
    idx_t lo = 1, hi = current_size;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (prefix[mid] >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = lo; /* row containing the nth element */

    /* Position of the desired divisor within this row (0‑based). */
    seq_t prev_sum = prefix[row - 1];
    seq_t pos_in_row = (seq_t)n - prev_sum - (seq_t)1;

    /* Find the pos_in_row‑th divisor of `row` that is ≤ √row. */
    idx_t limit = isqrt_idx(row);
    seq_t count = 0;
    seq_t result = (seq_t)0;

    for (idx_t d = 1; d <= limit; ++d) {
        if (row % d == 0) {
            if (count == pos_in_row) {
                result = (seq_t)d;
                break;
            }
            ++count;
        }
    }
    return result;
}
