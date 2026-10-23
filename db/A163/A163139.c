/*
 * A163139  First differences of A163128.
 * https://oeis.org/A163139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A163128(n+1) - A163128(n) = A003052(n+1) - A003052(n) - 1
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Self‑number generation is based on marking every number of the form
 *  i + sumdigits(i).  The array 'self_arr' holds the self numbers in
 *  ascending order.  It is grown until the requested index is present.
 */

#include <stdlib.h>
#include <stdint.h>

/* sum of decimal digits of an idx_t value */
static idx_t sum_digits(idx_t x)
{
    idx_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Global data structures */
static seq_t *self_arr   = NULL;   /* array of self numbers   */
static size_t self_len   = 0;      /* number of elements in self_arr   */
static size_t self_cap   = 0;      /* current allocation capacity */

static uint8_t *marked    = NULL;  /* 0‑based boolean array (size = limit+1) */
static size_t  marked_cap = 0;     /* current capacity of marked array */

static idx_t current_limit = 0;    /* last limit used in generate_to_limit */

/* Generate self numbers up to 'limit' (inclusive).  Produces exactly
 * the set of numbers ≤ limit that are not of the form i + sumdigits(i). */
static void generate_to_limit(idx_t limit)
{
    if (limit < 1) limit = 1;

    /* Allocate or resize the marked array.  Use uint8_t for minimal size. */
    if (marked_cap < (size_t)limit + 1) {
        free(marked);
        marked = (uint8_t *)calloc((size_t)limit + 1, 1);
        marked_cap = (size_t)limit + 1;
    } else {                /* clear the used part */
        size_t i, cap = (size_t)limit + 1;
        for (i = 1; i < cap; ++i) marked[i] = 0;
    }

    /* Mark all generators g = i + sumdigits(i) that are ≤ limit. */
    idx_t i;
    for (i = 1; i <= limit; ++i) {
        idx_t g = i + sum_digits(i);
        if (g <= limit) marked[g] = 1;
    }

    /* Allocate a new self array or reuse the old one if large enough. */
    size_t cap = (size_t)limit + 1;
    if (self_cap < cap) {
        free(self_arr);
        self_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        self_cap = cap;
    }

    /* Collect the self numbers in ascending order. */
    size_t cnt = 0;
    for (i = 1; i <= limit; ++i) {
        if (!marked[i]) {
            self_arr[cnt++] = (seq_t)i;
        }
    }
    self_len = cnt;
    current_limit = limit;
}

/* Ensure that we have at least (n+1) self numbers stored. */
static void ensure_self(idx_t n)
{
    /* We need indices 0 … n (total of n+1 elements). */
    while (self_len <= n) {
        idx_t new_limit;
        if (current_limit == 0) {
            new_limit = (n + 1) * 4 + 10;   /* rough upper bound */
            if (new_limit < 4) new_limit = 4;
        } else {
            new_limit = current_limit * 2;   /* double the previous limit */
            if (new_limit == 0) new_limit = (n + 1) * 4 + 10;
        }
        generate_to_limit(new_limit);
    }
}

/* Public function specified by the problem statement. */
seq_t a(idx_t n)
{
    /* n is 1‑based index of the OEIS sequence.  We need the
     * (n‑1)‑th and n‑th self numbers (0‑based). */
    if (n < 1) return (seq_t)0;   /* out of range: behaviour undefined */

    ensure_self(n);   /* guarantees self_arr has at least indices 0 … n */

    seq_t prev = self_arr[n - 1];
    seq_t curr = self_arr[n];
    return curr - prev - (seq_t)1;
}
