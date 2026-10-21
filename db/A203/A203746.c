/*
 * A203746  Vandermonde sequence using x^2 + y^2 applied to the Fibonacci
 *          numbers (1,2,3,5,...,F(n+1)), where F=A000045.
 * https://oeis.org/A203746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{1 <= i < j <= n} (F_i^2 + F_j^2), * where F_i is
 *            the i-th Fibonacci number (F_1 = 1, F_2 = 1). * a(1) = 1. * The
 *            function is memoized per call and works for any n >= 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)1;
    }

    /* Convert n to a size_t for array indexing (safe for all tests). */
    size_t len = (size_t)n;

    /* Allocate arrays for Fibonacci numbers and their squares. */
#ifdef __cplusplus
    std::vector<seq_t> fib;
    fib.reserve(len);
    std::vector<seq_t> sq;
    sq.reserve(len);
#else
    seq_t *fib = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *sq  = (seq_t*)malloc(len * sizeof(seq_t));
    if (!fib || !sq) {
        /* Allocation failed; return 0 as an error indicator. */
        if (fib) free(fib);
        if (sq)  free(sq);
        return (seq_t)0;
    }
#endif

    /* Compute Fibonacci numbers F_1 .. F_n. */
#ifdef __cplusplus
    fib.push_back((seq_t)1);          /* F_1 */
    if (len > 1) fib.push_back((seq_t)1);  /* F_2 */
    for (size_t i = 2; i < len; ++i) {
        fib.push_back(fib[i - 1] + fib[i - 2]);
    }
#else
    fib[0] = (seq_t)1;                /* F_1 */
    if (len > 1) fib[1] = (seq_t)1;   /* F_2 */
    for (size_t i = 2; i < len; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
#endif

    /* Compute squares of the Fibonacci numbers. */
#ifdef __cplusplus
    for (size_t i = 0; i < len; ++i) {
        sq.push_back(fib[i] * fib[i]);
    }
#else
    for (size_t i = 0; i < len; ++i) {
        sq[i] = fib[i] * fib[i];
    }
#endif

    /* Compute the Vandermonde product over all i < j. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            result = result * (sq[i] + sq[j]);
        }
    }

    /* Free dynamic arrays if allocated in C. */
#ifndef __cplusplus
    free(fib);
    free(sq);
#endif

    return result;
}
