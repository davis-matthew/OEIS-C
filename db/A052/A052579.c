/*
 * A052579  E.g.f. (2+x+x^2)/((1-x)(1+x+x^2)).
 * https://oeis.org/A052579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * f[n mod 3] with f = {2, 1, 1}. The recurrence *
 *            a(n+3) = (n+1)(n+2)(n+3) * a(n) follows from this definition
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* periodic multiplier */
    static const seq_t f[3] = { (seq_t)2, (seq_t)1, (seq_t)1 };

    /* cache of factorials: facts[i] = i! , fact_len = number of stored entries */
    static seq_t *facts = NULL;
    static idx_t fact_len = 0;          /* 0 means uninitialised */

    /* initialise cache for 0! */
    if (fact_len == 0) {
        fact_len = 1;                   /* will hold index 0 */
        facts = (seq_t *)malloc(sizeof(seq_t));
        if (!facts) abort();
#if defined(__cplusplus)
        new (facts) seq_t(1);           /* construct 0! = 1 */
#else
        facts[0] = (seq_t)1;
#endif
    }

    /* extend cache up to n if necessary */
    if (n + 1 > fact_len) {
        idx_t new_len = n + 1;           /* total entries needed */
        seq_t *tmp = (seq_t *)realloc(facts, new_len * sizeof(seq_t));
        if (!tmp) abort();
        facts = tmp;

        /* compute new factorials */
        for (idx_t i = fact_len; i <= n; ++i) {
            seq_t prev = facts[i - 1];
            seq_t mul = (seq_t)i;
#if defined(__cplusplus)
            new (facts + i) seq_t(prev * mul);  /* placement new for C++ */
#else
            facts[i] = prev * mul;
#endif
        }
        fact_len = new_len;
    }

    /* final value */
    return facts[n] * f[n % 3];
}
