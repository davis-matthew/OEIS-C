/*
 * A128519  McKay-Thompson series of class 78B for the Monster group with a(0)
 *          = -1.
 * https://oeis.org/A128519
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 94, i16 n <= 256, i32 n <= 780, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A128519 sequence – McKay‑Thompson series of class 78B
 *
 *  Formula: a(n) is the coefficient of q^n in
 *      q^{-1} * ∏_{k≥1, gcd(k,78)=1} (1 - q^k)
 *
 *  Implementation:  For each n, build the product from scratch up to
 *  exponent n+1.  The product contains only factors (1 - q^k) for
 *  k coprime to 78; all other k contribute the factor 1 and are
 *  ignored.  Coefficients are updated in place using a reverse
 *  loop to avoid interference with freshly updated entries.
 *
 *  The array of coefficients is cached so that increasing the
 *  requested n only recomputes the coefficients once.
 */

#include <stddef.h>
#include <stdlib.h>

/* Euclidean gcd for positive idx_t values */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* cached coefficient array: coeff[m] = coeff of q^m in the product.
   The array always holds at least indices 0..max_target-1, where
   max_target = current maximum (n+2) that has been computed. */
static seq_t *coeff = NULL;
static size_t  cap   = 0;

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)-1)
        return (seq_t)1;          /* a(-1) = 1 */

    if (n < (idx_t)-1)
        return (seq_t)0;          /* below offset – undefined, return 0 */

    /* We need coefficients up to index n+1 (i.e. array size n+2). */
    const size_t target = (size_t)(n + 2);

    if (target > cap) {
        /* Allocate or grow the coefficient array. */
        seq_t *new_arr = (seq_t *)calloc(target, sizeof(seq_t));
        if (new_arr == NULL)
            exit(1);

        /* First coefficient (q^0) is 1. */
        new_arr[0] = (seq_t)1;

        /* Build all remaining terms. */
        for (size_t k = 1; k < target; ++k) {
            if (gcd_idx((idx_t)k, (idx_t)78) == 1) {
                /* Multiply current product by (1 - q^k). */
                for (idx_t m = (idx_t)(target - 1); m >= (idx_t)k; --m) {
                    new_arr[(size_t)m] =
                        new_arr[(size_t)m] - new_arr[(size_t)(m - k)];
                }
            }
        }

        free(coeff);
        coeff = new_arr;
        cap   = target;
    }

    return coeff[(size_t)(n + 1)];
}
