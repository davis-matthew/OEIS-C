/*
 * A024861  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers), t = (F(2), F(3), F(4), ... ).
 * https://oeis.org/A024861
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} i * F(n-i+2) (F(0)=0, F(1)=1)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fib_cache = NULL;   /* Fibonacci cache  [0 .. fib_cache_len-1] */
static idx_t fib_cache_len = 0;   /* number of initialized elements */

/* Ensure fib_cache contains F(0) … F(upto) */
static void ensure_fib(idx_t upto)
{
    if (fib_cache_len > upto) return;                       /* already done */

    size_t new_len = (size_t)(upto + 1);
    seq_t *new_arr;

    if (fib_cache == NULL) {
        new_arr = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_arr) exit(1);
        new_arr[0] = (seq_t)0;
        if (upto >= 1) new_arr[1] = (seq_t)1;
        fib_cache_len = 2;
    } else {
        new_arr = (seq_t *)realloc(fib_cache, new_len * sizeof(seq_t));
        if (!new_arr) exit(1);
    }

    /* Fill missing Fibonacci values */
    for (idx_t i = fib_cache_len; i <= upto; ++i) {
        new_arr[i] = new_arr[i-1] + new_arr[i-2];
    }
    fib_cache = new_arr;
    fib_cache_len = upto + 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* offset: values below 2 are undefined */
    idx_t k = n / 2;              /* floor(n/2) */
    idx_t max_fib_idx = n + 1;    /* need F(n-i+2) with i≥1 → up to n+1 */
    ensure_fib(max_fib_idx);

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t coeff = (seq_t)i;
        idx_t fib_idx = n - i + 2;
        sum += coeff * fib_cache[fib_idx];
    }
    return sum;
}
