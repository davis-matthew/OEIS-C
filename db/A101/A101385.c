/*
 * A101385  Array read by antidiagonals: T(n,k) = variant of Knuth's Fibonacci
 *          (or circle) product of n and k (A101330).
 * https://oeis.org/A101385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A101385:
   Read the array T(m,n), where  
   T(m,n) = Σ_{i,j} eps(i)*eps(j)*Fib_{i*j}  
   (eps(i)=1 iff Fib_i appears in the Zeckendorf expansion of m, i>=2).
   The array is read by antidiagonals, starting with T(1,1). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑compute Fibonacci numbers up to index 200 (sufficient for n ≤ 10000). */
    enum { MAX_FIB = 200 };
    static seq_t fib[MAX_FIB + 1];
    static int fib_ready = 0;
    if (!fib_ready) {
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i <= MAX_FIB; ++i)
            fib[i] = fib[i-1] + fib[i-2];
        fib_ready = 1;
    }

    /* Find the diagonal d such that d*(d+1)/2 ≥ n. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;

    /* Convert n to coordinates (i, j) on the array. */
    idx_t prev = (d - 1) * d / 2;      /* elements before this diagonal */
    idx_t offset = n - prev;           /* 1‑based position in the diagonal */
    idx_t i = offset;                  /* row number  */
    idx_t j = d - i + 1;               /* column number */

    /* Zeckendorf representation of i (indices of Fibonacci numbers used). */
    idx_t rep_i[32];
    idx_t ci = 0;
    idx_t rem = i;
    for (int k = MAX_FIB; k >= 2; --k) {
        if (fib[k] <= (seq_t)rem) {
            rep_i[ci++] = k;
            rem -= (idx_t)fib[k];
        }
    }

    /* Zeckendorf representation of j. */
    idx_t rep_j[32];
    idx_t cj = 0;
    rem = j;
    for (int k = MAX_FIB; k >= 2; --k) {
        if (fib[k] <= (seq_t)rem) {
            rep_j[cj++] = k;
            rem -= (idx_t)fib[k];
        }
    }

    /* Compute the product sum. */
    seq_t result = 0;
    for (idx_t ii = 0; ii < ci; ++ii) {
        for (idx_t jj = 0; jj < cj; ++jj) {
            idx_t prod = rep_i[ii] * rep_j[jj];
            result += fib[prod];
        }
    }

    return result;
}
