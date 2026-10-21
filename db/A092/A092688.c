/*
 * A092688  Row sums of triangle A092686, in which the convolution of each row
 *          with {1,2} produces a triangle that, when flattened, equals the
 *          flattened form of A092686.
 * https://oeis.org/A092688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n) = 3*a(n-1) + b(n) - b(n-1), where b(0)=1
 *            and for n>=1: b(n) = 2*b(n-1) + sum_{k=1}^{⌊n/2⌋} C(n-2k, k) *
 *            b(k) * 2^{n-2k}
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache of values and helper arrays */
static seq_t *b_arr = NULL;          /* b(n) values, index 0..N */
static seq_t *a_arr = NULL;          /* a(n) values, index 0..N */
static seq_t *pow2_arr = NULL;       /* powers of two, pow2_arr[k] = 2^k */
static size_t  arr_size = 0;         /* allocated size of arrays      */
static idx_t   max_computed = (idx_t)(-1); /* last index computed */

/* Grow static arrays to hold at least indices 0..n
   (also initialise needed powers of two). */
static void ensure_arrays(idx_t n)
{
    if ((size_t)n < arr_size)
        return;                     /* already big enough */

    size_t new_size = (size_t)(n + 1);

    b_arr     = (seq_t*)realloc(b_arr,     new_size * sizeof(seq_t));
    a_arr     = (seq_t*)realloc(a_arr,     new_size * sizeof(seq_t));
    pow2_arr  = (seq_t*)realloc(pow2_arr,  new_size * sizeof(seq_t));
    if (!b_arr || !a_arr || !pow2_arr)
        abort();                      /* allocation failure */

    /* initialise pow2_arr entries as needed */
    size_t start = (arr_size == 0) ? 1 : arr_size;
    if (arr_size == 0)                  /* set 2^0 */
        pow2_arr[0] = (seq_t)1;
    for (size_t i = start; i < new_size; ++i)
        pow2_arr[i] = pow2_arr[i - 1] * ((seq_t)2);

    arr_size = new_size;

    /* initialise a[0] and b[0] on first call */
    if (max_computed == (idx_t)(-1)) {
        b_arr[0] = (seq_t)1;
        a_arr[0] = (seq_t)1;
        max_computed = 0;
    }
}

/* Compute all b(n) and a(n) up to the requested n. */
static void compute_up_to(idx_t n)
{
    /* ensure cache is initialised if this is the first call */
    if (max_computed == (idx_t)(-1))
        ensure_arrays(0);

    if (max_computed >= n)
        return;

    ensure_arrays(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t d = (seq_t)0;
        idx_t max_k = i / 2;
        if (max_k >= 1) {
            seq_t T = (seq_t)1;          /* C(i-2,0)=1 for k=1 */
            for (idx_t k = 1; k <= max_k; ++k) {
                idx_t e = i - 2 * k;          /* exponent for 2^{n-2k} */
                seq_t term = b_arr[k];
                term = term * pow2_arr[e];
                term = term * T;
                d += term;

                /* Update T for next k (if any) */
                if (k < max_k) {
                    seq_t num = T * (seq_t)(i - 2 * k);
                    num = num * (seq_t)(i - 2 * k - 1);
                    T = num / (((seq_t)k) * (seq_t)(i - k - 1));
                }
            }
        }
        seq_t bi = d + ((seq_t)2 * b_arr[i - 1]);
        b_arr[i] = bi;

        seq_t ai;
        if (i == 0) {
            ai = bi;
        } else {
            /* a[n] = 3*a[n-1] + b[n] - b[n-1] */
            ai = ((seq_t)3 * a_arr[i - 1]) + bi - b_arr[i - 1];
        }
        a_arr[i] = ai;
    }

    max_computed = n;
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_arr[n];
}
