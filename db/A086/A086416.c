/*
 * A086416  Number of divisors of 3-smooth numbers.
 * https://oeis.org/A086416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(2^(i_n) · 3^(j_n)), where 2^(i_n) · 3^(j_n) is the n‑th
 *            3‑smooth number (n ≥ 1)
 * Range:     u8 n <= 487, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Internal storage for exponents of 2 and 3 and the divisor counts.  */
static idx_t *exp2_arr = NULL;   /* exponent of 2  */
static idx_t *exp3_arr = NULL;   /* exponent of 3  */
static seq_t *tau_arr  = NULL;   /* τ = (i+1)(j+1)  */
static size_t  cap     = 0;      /* allocated length  */
static size_t  len     = 0;      /* number of terms stored (0‑based)  */
static idx_t  ptr2     = 0;      /* next index for multiplying by 2  */
static idx_t  ptr3     = 0;      /* next index for multiplying by 3  */

/* Forward declaration */
static void extend(idx_t n);

/* Ensure capacity for at least new_cap elements.  */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap)
        return;

    size_t grow = cap ? cap : 16;
    while (grow < new_cap)
        grow <<= 1;

    exp2_arr = (idx_t *)realloc(exp2_arr, grow * sizeof(idx_t));
    exp3_arr = (idx_t *)realloc(exp3_arr, grow * sizeof(idx_t));
    tau_arr  = (seq_t *)realloc(tau_arr,  grow * sizeof(seq_t));

    if (!exp2_arr || !exp3_arr || !tau_arr)
        abort();               /* allocation failed */
    cap = grow;
}

/* Compute terms up to (at least) the n‑th 3‑smooth number (1‑based). */
static void extend(idx_t n)
{
    if (n == 0 || len >= n)
        return;             /* already computed */

    ensure_capacity((size_t)n);

    if (len == 0) {          /* initialise first element  */
        exp2_arr[0] = 0;
        exp3_arr[0] = 0;
        tau_arr[0]  = (seq_t)(1);   /* (0+1)*(0+1) */
        len = 1;
        ptr2 = ptr3 = 0;
    }

    /* constants for comparing 2^a 3^b values */
    static double LOG2 = 0.0, LOG3 = 0.0;
    static int log_inited = 0;
    if (!log_inited) {
        LOG2 = log(2.0);
        LOG3 = log(3.0);
        log_inited = 1;
    }

    while (len < n) {
        idx_t a2 = exp2_arr[ptr2], b2 = exp3_arr[ptr2];
        idx_t a3 = exp2_arr[ptr3], b3 = exp3_arr[ptr3];

        /* equal case: 2^(a2+1)·3^b2  ==  2^a3·3^(b3+1) */
        if (a2 + 1 == a3 && b2 == b3 + 1) {
            exp2_arr[len] = a2 + 1;
            exp3_arr[len] = b2;
            ptr2++; ptr3++;               /* consume both candidates */
        } else {
            double v2 = (double)(a2 + 1) * LOG2 + (double)b2 * LOG3;
            double v3 = (double)a3 * LOG2 + (double)(b3 + 1) * LOG3;

            if (v2 < v3) {
                exp2_arr[len] = a2 + 1;
                exp3_arr[len] = b2;
                ptr2++;                    /* advance 2‑mul chain */
            } else {
                exp2_arr[len] = a3;
                exp3_arr[len] = b3 + 1;
                ptr3++;                    /* advance 3‑mul chain */
            }
        }

        /* τ = (i+1)(j+1) */
        tau_arr[len] = ((seq_t)(exp2_arr[len] + 1)) *
                       ((seq_t)(exp3_arr[len] + 1));
        len++;
    }
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)(0);            /* undefined, but defined safety */
    extend(n);
    return tau_arr[n - 1];
}
