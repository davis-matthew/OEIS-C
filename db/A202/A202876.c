/*
 * A202876  Symmetric matrix based on A000071, by antidiagonals.
 * https://oeis.org/A202876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry on the n-th antidiagonal of the infinite
 *            symmetric matrix M = T' * T, where T is formed from the sequence
 *            s(n) = Fibonacci(n+2) - 1. The n-th term is the entry (i,j) with
 *            i+j = D, D minimal such that D*(D-1)/2 ≥ n, enumerated by i
 *            increasing. Implementation uses the direct defin...
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset is 1; undefined for n<1 */

    /* Find D such that D*(D-1)/2 >= n */
    idx_t lo = 1;
    /* upper bound from the quadratic formula, add margin */
    idx_t hi = (idx_t)(sqrt(2.0 * (double)n) + 3.0);
    if (hi < 2) hi = 2;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t D = lo;

    /* Compute the number of terms before the current diagonal */
    unsigned __int128 prev_total128 = (unsigned __int128)(D - 1) * (D - 2) / 2;
    idx_t prev_total = (idx_t)prev_total128;

    /* Offset within the diagonal */
    idx_t offset = n - prev_total - 1;   /* 0‑based offset */
    idx_t i = offset + 1;
    idx_t j = D - i;

    /* Prepare s[1..max(i,j)] via Fibonacci numbers */
    idx_t L = i > j ? i : j;
    if (L == 0) return 0;  /* safety, should not happen */

    seq_t *fib = (seq_t *)malloc((L + 3) * sizeof(seq_t));
    seq_t *s   = (seq_t *)malloc((L + 1) * sizeof(seq_t));
    if (!fib || !s) {
        /* Memory allocation failed; return 0 as a safe fallback */
        free(fib);
        free(s);
        return 0;
    }

    fib[1] = 1;
    fib[2] = 1;
    for (idx_t k = 3; k <= L + 2; ++k)
        fib[k] = fib[k - 1] + fib[k - 2];

    for (idx_t k = 1; k <= L; ++k)
        s[k] = fib[k + 2] - 1;   /* s[n] = Fib(n+2) - 1 */

    /* Compute M[i,j] = sum_{k=1}^{min(i,j)} s[i-k+1] * s[j-k+1] */
    seq_t result = 0;
    idx_t min_ij = i < j ? i : j;
    for (idx_t k = 1; k <= min_ij; ++k)
        result += s[i - k + 1] * s[j - k + 1];

    free(s);
    free(fib);

    return result;
}
