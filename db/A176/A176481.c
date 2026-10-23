/*
 * A176481  Triangle, read by rows, defined by T(n, k) = b(n) - b(k) - b(n-k)
 *          + 2, where b(n) = A001333(n).
 * https://oeis.org/A176481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n-th element of the triangle T(r,k) where n =
 *            r(r+1)/2 + k (0 ≤ k ≤ r) and T(r,k) = b(r) - b(k) - b(r-k) + 2,
 *            with b(0)=b(1)=1 and b(i)=2*b(i-1)+b(i-2)
 * Range:     u8 n <= 36, u16 n <= 105, u32 n <= 354, u64 n <= 1378,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *b_cache = NULL;
static idx_t  b_len   = 0;
static size_t b_cap   = 0;

/* Ensure b_cache has values up to n, using recurrence b(0)=b(1)=1. */
static void ensure_b_upto(idx_t n)
{
    if (n < b_len) return;          /* already computed */

    /* allocate or grow storage */
    if (b_cap < n + 1) {
        /* double capacity strategy */
        size_t new_cap = (b_cap > 0) ? b_cap * 2 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        b_cap   = new_cap;
    }

    /* initialize first two terms if needed */
    if (b_len == 0) {
        b_cache[0] = 1;
        b_len = 1;
        if (n == 0) return;
    }
    if (b_len == 1) {
        b_cache[1] = 1;
        b_len = 2;
        if (n == 1) return;
    }

    /* compute remaining terms */
    for (idx_t i = b_len; i <= n; ++i) {
        seq_t t = b_cache[i - 1];
        t *= 2;
        t += b_cache[i - 2];
        b_cache[i] = t;
    }
    b_len = n + 1;
}

/* Find the row r such that C(r) = (r+1)(r+2)/2 > n. */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t c   = ((mid + 1) * (mid + 2)) / 2;
        if (c <= n) low = mid + 1;
        else        high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from linear index n */
    idx_t r = find_row(n);
    idx_t prev_sum = (r * (r + 1)) / 2;   /* sum of lengths of rows before r */
    idx_t k = n - prev_sum;

    /* Make sure b values up to r are available */
    ensure_b_upto(r);

    /* Compute T(r,k) = b(r) - b(k) - b(r-k) + 2 */
    seq_t val = b_cache[r];
    val -= b_cache[k];
    val -= b_cache[r - k];
    val += 2;

    return val;
}
