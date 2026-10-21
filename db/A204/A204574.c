/*
 * A204574  Numbers such that floor[a(n)^2/2] is a square (A001541), written
 *          in binary.
 * https://oeis.org/A204574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0; for n>=2, a(n) is the decimal value obtained by *
 *            treating the binary representation of the (n‑2)th Pell number *
 *            x_{k} (with (x_{0},y_{0})=(1,0) and
 *            (x_{k+1},y_{k+1})=(3x_{k}+4y_{k}, * 2x_{k}+3y_{k})) as a base‑10
 *            digit string
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_seq_array, bitlen_seq_t, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

/* Allocate and free functions for seq_t arrays (provided by the build) */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t*) */

static seq_t *X_arr = NULL;   /* array of Pell x-values   */
static seq_t *Y_arr = NULL;   /* array of Pell y-values   */
static size_t  X_cap = 0;     /* current capacity of arrays */
static size_t  X_len = 0;     /* number of filled entries   */

seq_t a(idx_t n)
{
    if (n == 1)            /* special offset value */
        return (seq_t)0;

    /* Index for Pell sequence: x_{k} with k = n-2 (k >= 0) */
    size_t k = (size_t)(n - 2);

    /* Initialise storage on first use */
    if (X_len == 0) {
        X_cap = 1;
        X_arr = alloc_seq_array(X_cap);
        Y_arr = alloc_seq_array(X_cap);
        X_arr[0] = (seq_t)1; /* (x0, y0) = (1, 0) */
        Y_arr[0] = (seq_t)0;
        X_len = 1;
    }

    /* Grow arrays if needed */
    if (X_len <= k) {
        size_t needed = k + 1;
        if (X_cap < needed) {
            size_t new_cap = X_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *new_X = alloc_seq_array(new_cap);
            seq_t *new_Y = alloc_seq_array(new_cap);
            for (size_t i = 0; i < X_len; ++i) {
                new_X[i] = X_arr[i];
                new_Y[i] = Y_arr[i];
            }
            free_seq_array(X_arr);
            free_seq_array(Y_arr);
            X_arr = new_X;
            Y_arr = new_Y;
            X_cap = new_cap;
        }

        /* Compute missing Pell terms up to k */
        for (size_t i = X_len; i <= k; ++i) {
            seq_t px = X_arr[i-1];
            seq_t py = Y_arr[i-1];
            X_arr[i] = px * 3 + py * 4;
            Y_arr[i] = px * 2 + py * 3;
        }
        X_len = k + 1;
    }

    /* Convert binary representation of x_k to a decimal digit string */
    seq_t xk = X_arr[k];
    seq_t result = (seq_t)0;
    size_t len = bitlen_seq_t(xk);
    for (size_t pos = len; pos-- > 0; ) {
        seq_t bit = (xk >> pos) & ((seq_t)1);
        result = result * 10 + bit;
    }
    return result;
}
