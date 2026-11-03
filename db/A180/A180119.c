/*
 * A180119  a(n) = (n+2)! * Sum_{k = 1..n} 1/((k+1)*(k+2)).
 * https://oeis.org/A180119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)!/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 297 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Static cache for factorial values up to the largest n requested. */
    static seq_t *fact = NULL;          /* array of factorials, fact[i] = i! */
    static size_t fact_cap = 0;         /* allocated capacity of the array   */
    static size_t fact_size = 1;        /* highest index computed + 1 (fact[0] is 0! = 1) */

    /* Ensure capacity for factorial of (n+1). */
    size_t need = (size_t)(n + 1);   /* we need fact[need] = (n+1)! */
    if (fact == NULL) {
        fact_cap = 1;
        fact = mem_new(seq_t, fact_cap);
        if (!fact) return 0;           /* allocation failed -> return 0 */
        fact[0] = 1;                   /* 0! = 1 */
    }
    if (need >= fact_cap) {
        size_t new_cap = fact_cap ? fact_cap : 1;
        while (new_cap <= need) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, fact, new_cap);
        if (!tmp) return 0;           /* allocation failed -> return 0 */
        fact = tmp;
        fact_cap = new_cap;
    }

    /* Compute any missing factorials up to need. */
    for (size_t i = fact_size; i <= need; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_size = need + 1;

    /* a(n) = n * (n+1)! / 2 */
    seq_t result = (seq_t)n * fact[need];
    result /= 2;
    return result;
}
