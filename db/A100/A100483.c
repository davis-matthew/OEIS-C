/*
 * A100483  a(1) = 2, thereafter a(n) = Sum_{k=1..n-1} floor(a(n-k)/k).
 * https://oeis.org/A100483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n-1} floor(a(n-k)/k), a(1) = 2
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 50, u64 n <= 98, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A100483. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *vals = NULL;
    static idx_t cap   = 0;
    static idx_t size  = 0;   /* number of terms already computed */

    /* initialise the first term if needed */
    if (size == 0) {
        cap  = 1;
        vals = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!vals) return (seq_t)0;    /* allocation failed, return zero */
        vals[0] = (seq_t)2;
        size = 1;
    }

    /* if n already computed, return it */
    if (n <= size) return vals[n-1];

    /* enlarge the array so that it can hold terms up to n */
    if (n > cap) {
        idx_t newcap = n;
        seq_t *tmp = (seq_t *)realloc(vals, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;    /* allocation failed */
        vals = tmp;
        cap  = newcap;
    }

    /* compute terms from (size+1) up to n */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            sum += vals[i-1-k] / (seq_t)k;  /* floor division */
        }
        vals[i-1] = sum;
    }
    size = n;
    return vals[n-1];
}
