/*
 * A100396  Bell(n-1) + Fibonacci(n).
 * https://oeis.org/A100396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n-1) + Fibonacci(n)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) is defined as 1 */

    /* static caches for Bell numbers and Fibonacci numbers */
    static seq_t *bell = NULL;
    static idx_t bell_len = 0;   /* number of Bell entries computed (highest index stored + 1) */

    static seq_t *fib = NULL;
    static idx_t fib_len = 0;    /* number of Fibonacci entries computed (highest index stored + 1) */

    /* Compute Bell numbers up to index n-1 if needed */
    if (bell_len <= n - 1) {
        idx_t newlen = n;   /* we want Bell[0..n-1] inclusive */
        if (bell == NULL) {
            bell = (seq_t*)malloc(sizeof(seq_t) * newlen);
            if (bell == NULL)   /* allocation failed */
                return (seq_t)0;
            bell[0] = (seq_t)1;   /* B0 = 1 */
            bell_len = 1;
        } else if (newlen > bell_len) {
            bell = (seq_t*)realloc(bell, sizeof(seq_t) * newlen);
            if (bell == NULL)
                return (seq_t)0;
        }

        for (idx_t i = bell_len; i < newlen; ++i) {
            seq_t sum = (seq_t)0;
            seq_t c   = (seq_t)1;         /* binomial coefficient C(i-1, 0) */
            idx_t kmax = i - 1;
            for (idx_t k = 0; k <= kmax; ++k) {
                sum += c * bell[k];
                if (k < kmax) {
                    seq_t mult = c * (seq_t)(kmax - k);
                    c = mult / (seq_t)(k + 1);   /* update C(i-1, k+1) */
                }
            }
            bell[i] = sum;
        }
        bell_len = newlen;
    }

    /* Compute Fibonacci numbers up to index n if needed */
    if (fib_len <= n) {
        idx_t newlen = n + 1;   /* we need fib[0..n] */
        if (fib == NULL) {
            fib = (seq_t*)malloc(sizeof(seq_t) * newlen);
            if (fib == NULL)
                return (seq_t)0;
            fib[0] = (seq_t)0;
            if (newlen > 1)
                fib[1] = (seq_t)1;
            fib_len = 2;
        } else if (newlen > fib_len) {
            fib = (seq_t*)realloc(fib, sizeof(seq_t) * newlen);
            if (fib == NULL)
                return (seq_t)0;
        }

        for (idx_t i = fib_len; i <= n; ++i) {
            if (i == 0)
                fib[i] = (seq_t)0;
            else if (i == 1)
                fib[i] = (seq_t)1;
            else
                fib[i] = fib[i-1] + fib[i-2];
        }
        fib_len = newlen;
    }

    /* a(n) = Bell(n-1) + Fibonacci(n) */
    return bell[n-1] + fib[n];
}
