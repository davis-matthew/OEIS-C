/*
 * A130711  Number of compositions of n such that the smallest part divides
 *          every part.
 * https://oeis.org/A130711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{d|n} ( 2^(d-1) – F_{d-1} ) where F_k is the k‑th
 *            Fibonacci number (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) for A130711.  */
#include <stdlib.h>

static seq_t *c_arr = NULL;   /* c[d] = 2^(d-1)-F_{d-1}   */
static seq_t *a_arr = NULL;   /* a[n] = Σ_{d|n} c[d]      */
static size_t a_size = 0;     /* current allocated size   */

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 by the problem specification. */
    size_t N = (size_t)n;

    if (N >= a_size) { /* need to expand the tables */
        size_t new_size = N + 1;   /* index 0 unused */
        seq_t *new_c = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *new_a = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *pow2  = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *fib   = (seq_t*)calloc(new_size, sizeof(seq_t));

        /* Fibonacci numbers: F_0 = 0, F_1 = 1 */
        fib[0] = 0;
        if (new_size > 1) fib[1] = 1;
        for (size_t i = 2; i < new_size; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];

        /* Powers of two: 2^i */
        pow2[0] = 1;
        for (size_t i = 1; i < new_size; ++i)
            pow2[i] = pow2[i - 1] * 2;

        /* Compute c[d] for all d */
        for (size_t d = 1; d < new_size; ++d) {
            seq_t val = pow2[d - 1];
            val -= fib[d - 1];    /* 2^(d-1) - F_{d-1} */
            new_c[d] = val;
        }

        /* Compute a[n] = Σ_{d|n} c[d]  via a divisor sieve */
        for (size_t d = 1; d < new_size; ++d) {
            seq_t val = new_c[d];
            for (size_t m = d; m < new_size; m += d)
                new_a[m] += val;
        }

        /* Free old memory */
        free(c_arr);
        free(a_arr);

        /* Adopt new tables */
        c_arr   = new_c;
        a_arr   = new_a;
        a_size  = new_size;

        free(pow2);
        free(fib);
    }

    return a_arr[N];
}
