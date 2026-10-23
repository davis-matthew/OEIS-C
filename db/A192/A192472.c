/*
 * A192472  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x)=1+x^n+x^(2n+2).
 * https://oeis.org/A192472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + F_{n-1} + F_{2n+1} where F_k is the k-th Fibonacci
 *            number (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* Ensure n >= 1; the sequence is defined from offset 1. */
    if (n == 0) return (seq_t)0;

#ifdef __cplusplus
    /* C++ implementation using std::vector for automatic construction */
    static std::vector<seq_t> fibs;
    if (fibs.size() < 2) {
        fibs.reserve(2);
        fibs.push_back((seq_t)0);   // F_0
        fibs.push_back((seq_t)1);   // F_1
    }

    idx_t required = 2 * n + 1;          // we need F_{2n+1}
    while ((idx_t)fibs.size() <= required) {
        size_t sz = fibs.size();
        fibs.push_back(fibs[sz - 1] + fibs[sz - 2]);
    }

    seq_t result = (seq_t)1 + fibs[n - 1] + fibs[required];
    return result;
#else
    /* C implementation using a dynamically allocated array */
    static seq_t *fibs = NULL;
    static idx_t  fib_count = 0;    /* number of Fibonacci numbers stored */
    static size_t fib_cap   = 0;    /* capacity of the array */

    /* Initialize storage on first call */
    if (fibs == NULL) {
        fib_cap   = 2;
        fib_count = 2;
        fibs      = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        if (!fibs) abort();
        fibs[0] = (seq_t)0;   /* F_0 */
        fibs[1] = (seq_t)1;   /* F_1 */
    }

    idx_t required = 2 * n + 1;   /* we need F_{2n+1} */

    /* Grow array if necessary */
    if (required >= fib_cap) {
        size_t new_cap = fib_cap ? fib_cap * 2 : 2;
        while (new_cap <= (size_t)required) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(fibs, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fibs   = tmp;
        fib_cap = new_cap;
    }

    /* Compute missing Fibonacci numbers */
    while (fib_count <= required) {
        fibs[fib_count] = fibs[fib_count - 1] + fibs[fib_count - 2];
        ++fib_count;
    }

    seq_t result = (seq_t)1 + fibs[n - 1] + fibs[required];
    return result;
#endif
}
