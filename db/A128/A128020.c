/*
 * A128020  a(n) = the multiple of n which is > (sum{k=1 to n-1} a(k)) and is
 *          <= (n + sum{k=1 to n-1} a(k)).
 * https://oeis.org/A128020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * (1 + floor((sum_{k=1}^{n-1} a(k)) / n))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for values a(1)..a[max_cache] and cumulative sums */
    static seq_t *a_seq   = NULL;      /* a_seq[1]..a_seq[max_cache]   */
    static seq_t *sum_seq = NULL;      /* sum_seq[0]=0, sum_seq[1..]    */
    static idx_t max_cache     = 0;    /* largest index allocated      */
    static idx_t last_computed = 0;    /* largest n already filled      */

    /* if the caller requests a(0) return 0 – outside the intended offset */
    if (n == 0)
        return (seq_t)0;

    /* allocate or grow the cache if necessary */
    if (max_cache < n) {
        idx_t new_cap = max_cache ? max_cache * 2 : 1;
        if (new_cap < n)
            new_cap = n;

        /* realloc returns void*; cast to seq_t* */
        seq_t *tmp_a   = (seq_t *)realloc(a_seq,   (new_cap + 1) * sizeof(seq_t));
        seq_t *tmp_sum = (seq_t *)realloc(sum_seq, (new_cap + 1) * sizeof(seq_t));
        if (!tmp_a || !tmp_sum) { /* allocation failure – abort */
            if (tmp_a) free(tmp_a);
            if (tmp_sum) free(tmp_sum);
            return (seq_t)0;
        }
        a_seq   = tmp_a;
        sum_seq = tmp_sum;

        /* initialise the newly allocated slots */
        for (idx_t i = max_cache + 1; i <= new_cap; ++i) {
            a_seq[i]   = (seq_t)0;
            sum_seq[i] = (seq_t)0;
        }
        max_cache = new_cap;
    }

    /* initialise first term a(1) if this is the first call */
    if (last_computed == 0) {
        a_seq[1]   = (seq_t)1;
        sum_seq[0] = (seq_t)0;
        sum_seq[1] = a_seq[1];
        last_computed = 1;
    }

    /* compute any missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t s_prev = sum_seq[i - 1];
        seq_t t = s_prev / (seq_t)i;                /* floor division */
        a_seq[i] = (seq_t)i * (t + (seq_t)1);       /* a(i) = i * (1 + floor(S(i-1)/i)) */
        sum_seq[i] = sum_seq[i - 1] + a_seq[i];
    }
    last_computed = n;

    return a_seq[n];
}
