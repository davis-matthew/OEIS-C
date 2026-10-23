/*
 * A134548  a(1)=1, a(n) = 2 + maximal digit of Sum_{j=1..n-1} a(j).
 * https://oeis.org/A134548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(1) = 1 a(n) = 2 + maximal digit of Σ_{j=1..n-1} a(j) (n > 1)
 *            (maximal digit is the largest decimal digit of the sum)
 * Range:     u8 n <= 33, u16 n <= 7703, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper for allocating and resising a seq_t array */
#include "../utils/seq_realloc.h"

static seq_t *a_vals   = NULL;   /* array of sequence values, 1‑based index (stored at 0) */
static size_t len      = 0;      /* number of values currently stored */
static size_t cap      = 0;      /* allocated capacity of a_vals */
static seq_t sum       = 0;      /* sum of all stored values a(1)..a(len) */

/* Ensure that a_vals has storage for at least 'need' elements.  */
static void ensure_capacity(size_t need)
{
    if (need <= cap)
        return;

    size_t new_cap = cap ? cap : 16;
    while (new_cap < need)
        new_cap <<= 1;          /* double until big enough */

    a_vals = seq_realloc(a_vals, cap, new_cap);
    cap    = new_cap;
}

/* Return the maximum decimal digit of a non‑negative seq_t value. */
static seq_t max_digit(seq_t x)
{
    seq_t ten = (seq_t)10;
    seq_t max = (seq_t)0;
    while (x > 0) {
        seq_t d = x % ten;
        if (d > max)
            max = d;
        x /= ten;
    }
    return max;
}

seq_t a(idx_t n)
{
    /* Guard against requests with n < 1; behavior unspecified in problem,
       but we return 0 to avoid UB. */
    if (n < 1)
        return (seq_t)0;

    /* Initialise the first term lazily. */
    if (!a_vals) {
        ensure_capacity(1);
        a_vals[0] = (seq_t)1;
        len = 1;
        sum = (seq_t)1;
    }

    /* Grow the sequence up to the requested n (there might be uncomputed
       terms left from earlier calls). */
    while (len < n) {
        seq_t next = ((seq_t)2) + max_digit(sum);
        ensure_capacity(len + 1);
        a_vals[len] = next;
        ++len;
        sum += next;
    }

    return a_vals[n - 1];
}
