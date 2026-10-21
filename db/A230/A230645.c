/*
 * A230645  Values of y such that x^2 + y^2 = 29^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A230645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence for A230645 – the larger coprime component in
    the representation x^2 + y^2 = 29^n (x < y).
    Let (x_k, y_k) be that pair.  Starting with (x_1, y_1) = (2, 5),
    for k ≥ 2 the Gaussian integer (x_{k-1} + i y_{k-1}) is
    multiplied alternately by (5 − 2i) twice, then by (5 + 2i) twice,
    which yields the recurrence

    block = ⌊(k − 2)/2⌋          // 0‑based block of two multiplications
    if block is even   // multiply by (5‑2i)
        a = 5*x + 2*y
        b = |5*y − 2*x|
    else                // multiply by (5+2i)
        a = |5*x − 2*y|
        b = 2*x + 5*y

    (x_k, y_k) = (min(a,b), max(a,b))

    The function returns y_k.  The sequence can be evaluated in
    O(n) with memoisation, works for arbitrary‑precision seq_t
    and for uint64_t with values that fit.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0‑based offset is 1 – values below 1 are undefined. */
    if (n < 1) return 0;

    /* Memoisation tables: 1‑indexed. */
    static idx_t max_computed = 0; /* largest n that has been computed */
    static size_t capacity   = 0;  /* allocated size of arrays      */
    static seq_t *X = NULL;        /* smaller component of pair      */
    static seq_t *Y = NULL;        /* larger component of pair      */

    /* Allocate or grow tables to at least n entries. */
    if (n > capacity) {
        size_t new_cap = capacity ? capacity : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;   /* double */

        seq_t *x_new = (seq_t *)realloc(X, (new_cap + 1) * sizeof(seq_t));
        seq_t *y_new = (seq_t *)realloc(Y, (new_cap + 1) * sizeof(seq_t));

        if (!x_new || !y_new) abort();   /* allocation failure */

        X = x_new; Y = y_new;
        capacity = new_cap;
    }

    /* Initialise the base case if necessary. */
    if (max_computed == 0) {
        X[1] = (seq_t)2;   /* x_1 */
        Y[1] = (seq_t)5;   /* y_1 */
        max_computed = 1;
    }

    /* Compute all missing terms up to n. */
    while (max_computed < n) {
        idx_t cur  = max_computed;     /* current computed index  */
        idx_t next = cur + 1;          /* index to be computed   */

        /* Determine which Gaussian multiplier to use. */
        idx_t block = (next - 2) / 2;   /* floor((next-2)/2) */
        seq_t x = X[cur];
        seq_t y = Y[cur];
        seq_t new_x, new_y;

        if ((block & 1) == 0) { /* (5 - 2i) */
            seq_t a_val = 5 * x + 2 * y;
            seq_t five_y = 5 * y;
            seq_t two_x  = 2 * x;
            seq_t b_val = (five_y >= two_x) ? (five_y - two_x) : (two_x - five_y);

            /* Arrange so that new_x < new_y */
            if (b_val < a_val) {
                new_x = b_val;
                new_y = a_val;
            } else {
                new_x = a_val;
                new_y = b_val;
            }
        } else { /* (5 + 2i) */
            seq_t five_x = 5 * x;
            seq_t two_y  = 2 * y;
            seq_t diff   = (five_x >= two_y) ? (five_x - two_y)
                                             : (two_y - five_x);
            seq_t imag   = 2 * x + 5 * y;

            /* Arrange so that new_x < new_y */
            if (imag < diff) {
                new_x = imag;
                new_y = diff;
            } else {
                new_x = diff;
                new_y = imag;
            }
        }

        X[next] = new_x;
        Y[next] = new_y;
        max_computed = next;
    }

    /* Return the larger component for the requested n. */
    return Y[n];
}
