/*
 * A091364  a(n) = n! * n^4.
 * https://oeis.org/A091364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * n^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized factorial cache */
static seq_t *fact_cache = NULL;   /* array of factorial values */
static idx_t fact_len   = 0;        /* number of computed entries */
static idx_t fact_cap   = 0;        /* current capacity */

/* Ensure factorials are computed up to index 'n' */
static void ensure_factorial(idx_t n) {
    while (fact_len <= n) {
        /* Grow capacity if needed */
        if (fact_len == fact_cap) {
            fact_cap = fact_cap ? fact_cap * 2 : 4;
            fact_cache = (seq_t *)realloc(fact_cache, fact_cap * sizeof(seq_t));
        }

        if (fact_len == 0) {
            /* fact_cache[0] = 0! = 1 */
            fact_cache[0] = (seq_t)1;
        } else {
            /* fact_cache[i] = fact_cache[i-1] * (i) */
            fact_cache[fact_len] = fact_cache[fact_len - 1] * (seq_t)fact_len;
        }
        fact_len++;
    }
}

/* Primary sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* 0! * 0^4 = 0 */

    ensure_factorial(n);                /* compute factorial up to n */

    /* Compute n^4 */
    seq_t nsq = (seq_t)n;               /* n^1 */
    nsq *= nsq;                         /* n^2 */
    nsq *= nsq;                         /* n^4 */

    return fact_cache[n] * nsq;         /* n! * n^4 */
}
