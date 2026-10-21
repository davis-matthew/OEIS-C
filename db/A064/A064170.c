/*
 * A064170  a(1) = 1; a(n+1) = product of numerator and denominator in
 *          Sum_{k=1..n} 1/a(k).
 * https://oeis.org/A064170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2n‑5) * Fibonacci(2n‑3) for n ≥ 3 with a(1) =
 *            1, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)1;   /* a(1) = a(2) = 1 */

    /* ---- Fibonacci cache ---- */
    static seq_t *fib = NULL;      /* fib[i] = F(i) for i ≥ 0   */
    static idx_t fib_len = 0;      /* number of entries currently computed  */
    static idx_t fib_cap = 0;      /* allocated capacity of fib array */

    /* needed Fibonacci indices */
    idx_t idx1 = 2 * n - 5;
    idx_t idx2 = 2 * n - 3;
    idx_t need = idx2;             /* the larger one is the maximum required */

    if (fib == NULL) {            /* first call – initialise */
        fib_cap = 8;               /* arbitrary small choice */
        fib_len = 2;               /* we know F(0) and F(1) */
        fib = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        if (!fib) return (seq_t)0; /* out of memory – unlikely in tests */
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
    }

    /* grow capacity if necessary */
    if (need >= fib_cap) {
        idx_t new_cap = fib_cap;
        while (new_cap <= need) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(fib, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        fib = tmp;
        fib_cap = new_cap;
    }

    /* compute any missing Fibonacci values up to 'need' */
    for (idx_t i = fib_len; i <= need; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    fib_len = need + 1;

    /* return the product of the required Fibonacci numbers */
    return fib[idx1] * fib[idx2];
}
