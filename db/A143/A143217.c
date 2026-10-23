/*
 * A143217  a(n) = n! * (!(n+1)) = n! * Sum_{k=0..n} k!.
 * https://oeis.org/A143217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * sum_{k=0..n} k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoization tables */
    static idx_t max_n = 0;
    static seq_t *fact = NULL, *sum = NULL, *val = NULL;
    static size_t cap = 0;

    /* initial allocation for index 0 */
    if (fact == NULL) {
        cap = 1;
        fact = (seq_t *)malloc(cap * sizeof(seq_t));
        sum  = (seq_t *)malloc(cap * sizeof(seq_t));
        val  = (seq_t *)malloc(cap * sizeof(seq_t));
        fact[0] = (seq_t)1;
        sum[0]  = (seq_t)1;
        val[0]  = fact[0] * sum[0];
        max_n   = 0;
    }

    /* extend tables up to requested n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* grow capacity as needed */
        if ((size_t)i >= cap) {
            size_t new_cap = cap > 0 ? cap * 2 : 1;
            while (new_cap <= (size_t)i) new_cap *= 2;
            fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
            sum  = (seq_t *)realloc(sum,  new_cap * sizeof(seq_t));
            val  = (seq_t *)realloc(val,  new_cap * sizeof(seq_t));
            cap  = new_cap;
        }

        /* compute factorial, partial sum, and value */
        fact[i] = fact[i - 1] * (seq_t)i;          /* (i-1)! * i = i! */
        sum[i]  = sum[i - 1] + fact[i];           /* sum_{k<=i} k!   */
        val[i]  = fact[i] * sum[i];               /* desired a(i)    */
    }
    max_n = n;

    return val[n];
}
