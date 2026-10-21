/*
 * A222179  Numbers n such that 9n is a partition number.
 * https://oeis.org/A222179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(k)/9 where p(k) is the nth partition number divisible
 *            by 9 (partition numbers are p(0)=1, p(1)=1, …)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 10, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *p = NULL;        /* partition numbers */
    static idx_t currentN = 0;     /* highest computed partition index   */
    static size_t p_cap   = 0;     /* capacity of array p                */
    static seq_t *a_vals = NULL;   /* list of a(j) values                */
    static idx_t maxComputed = 0;  /* number of a(j) values stored       */

    /* Lazy initialisation of partition array */
    if (p == NULL) {
        p_cap = 1;
        p = (seq_t*)malloc(p_cap * sizeof(seq_t));
        p[0] = (seq_t)1;                /* p(0) = 1 */
        currentN = 0;
    }

    /* Ensure the result array has space up to required index */
    if (a_vals == NULL) {
        a_vals = (seq_t*)malloc(128 * sizeof(seq_t));
        maxComputed = 0;
    }

    /* Compute as many elements as needed */
    while (maxComputed < n) {
        idx_t next_k = currentN + 1;

        /* Ensure capacity of partition array */
        if (next_k >= p_cap) {
            p_cap = (p_cap > 0) ? p_cap * 2 : 64;
            p = (seq_t*)realloc(p, p_cap * sizeof(seq_t));
        }

        /* Compute p[next_k] using the generalized pentagonal
           number recurrence, avoiding negative intermediates.   */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = (k * (3 * k - 1)) / 2;
            if (g1 > next_k) break;
            if (k & 1) pos += p[next_k - g1];
            else       neg += p[next_k - g1];

            idx_t g2 = (k * (3 * k + 1)) / 2;
            if (g2 > next_k) continue;
            if (k & 1) pos += p[next_k - g2];
            else       neg += p[next_k - g2];
        }

        p[next_k] = pos - neg;          /* pos >= neg always for partitions */
        currentN = next_k;

        /* Check divisibility by 9 and store the quotient. */
        if ( (p[next_k] % (seq_t)9) == 0 ) {
            if (maxComputed == 128) {
                a_vals = (seq_t*)realloc(a_vals, (maxComputed + 128) * sizeof(seq_t));
            }
            a_vals[maxComputed] = p[next_k] / (seq_t)9;
            ++maxComputed;
        }
    }

    return a_vals[n - 1];
}
