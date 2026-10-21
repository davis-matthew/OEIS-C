/*
 * A092684  First column and main diagonal of triangle A092683, in which the
 *          convolution of each row with {1,1} produces a triangle that, when
 *          flattened, equals the flattened form of A092683.
 * https://oeis.org/A092684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A092684
 *  a(0) = 1, a(1) = 1
 *  a(n) = Sum_{k=0..floor(n/2)} C(n-k,k) * a(k)
 *  where C(n,k) is the binomial coefficient.
 *
 *  Implementation: dynamic programming with the recurrence.
 *  Binomial coefficients are updated iteratively:
 *      b_{k+1} = b_k * (n-2k)*(n-2k-1) / ((n-k)*(k+1))
 *  All arithmetic is performed in the unlimited-precision type
 *  seq_t.  For safety we use unsigned __int128 for intermediate
 *  multiplications to avoid overflow for the 64‑bit build.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* uint64_t, etc. */

static seq_t *a_vals = NULL;   /* memoised values of the sequence  */
static idx_t a_size = 0;       /* number of stored values (>= a_size-1) */

/* Helper: compute values up to n, storing them in the static array. */
static void compute_up_to(idx_t n)
{
    if (n < a_size)
        return;              /* already computed */

    /* Allocate (or reallocate) enough space. */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_arr = (seq_t*)realloc(a_vals, new_size * sizeof(seq_t));
    if (new_arr == NULL)
        abort();              /* unrecoverable failure */
    a_vals = new_arr;

    /* Compute from the first missing index up to n. */
    for (idx_t i = a_size; i <= n; ++i) {
        if (i == 0 || i == 1) {
            a_vals[i] = (seq_t)1;
            continue;
        }

        idx_t maxk = i / 2;
        seq_t sum = 0;
        seq_t binom = 1;          /* C(i,0) */

        for (idx_t k = 0; k <= maxk; ++k) {
            /* sum += binom * a_vals[k]  */
            unsigned __int128 tmp_term = (unsigned __int128)binom * (unsigned __int128)a_vals[k];
            sum += (seq_t)tmp_term;

            if (k == maxk)
                break;

            /* Compute next binomial coefficient:
             * b_{k+1} = b_k * (i-2k)*(i-2k-1) / ((i-k)*(k+1)) */
            seq_t n1 = (seq_t)(i - 2 * k);
            seq_t n2 = (seq_t)(i - 2 * k - 1);
            seq_t d1 = (seq_t)(i - k);
            seq_t d2 = (seq_t)(k + 1);

            unsigned __int128 numerator   = (unsigned __int128)binom * (unsigned __int128)n1 * (unsigned __int128)n2;
            unsigned __int128 denominator = (unsigned __int128)d1 * (unsigned __int128)d2;
            binom = (seq_t)(numerator / denominator);
        }

        a_vals[i] = sum;
    }

    a_size = n + 1;
}

/*  Public function required by the test harness. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_vals[n];
}
