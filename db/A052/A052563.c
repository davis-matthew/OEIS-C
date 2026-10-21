/*
 * A052563  Expansion of e.g.f.: (1-x)/(1-3*x).
 * https://oeis.org/A052563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 for n = 0 * a(n) = 2 * 3^(n-1) * n! for n ≥ 1 (OEIS
 *            A052563) * The implementation uses static memoised tables for
 *            factorials * and powers of 3 to support repeated calls in any
 *            order
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case for the offset. */
    if (n == 0)
        return (seq_t)1;

    /* --- Memoised factorials ------------------------------------------ */
    static seq_t *fact      = NULL;   /* fact[i] = i!  (idx_t i) */
    static size_t fact_cap  = 0;      /* current capacity of 'fact' array */
    static idx_t  fact_len  = 0;      /* number of factorials stored (0..fact_len-1) */

    /* Grow factorial cache if necessary. */
    if ((size_t)n + 1 > fact_cap) {
        size_t new_cap = (fact_cap == 0) ? 64 : fact_cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        fact = tmp;
        fact_cap = new_cap;
    }

    /* Initialise factorial table if empty. */
    if (fact_len == 0) {
        fact[0] = (seq_t)1;
        fact_len = 1;
    }

    /* Compute factorials up to n. */
    for (idx_t i = fact_len; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_len = n + 1;

    /* --- Memoised powers of 3 ------------------------------------------ */
    static seq_t *pow3      = NULL;   /* pow3[i] = 3^i  (idx_t i) */
    static size_t pow3_cap  = 0;      /* capacity of 'pow3' array */
    static idx_t  pow3_len  = 0;      /* number of powers stored (0..pow3_len-1) */

    /* Grow power array if necessary. */
    if ((size_t)(n - 1) + 1 > pow3_cap) {
        size_t new_cap = (pow3_cap == 0) ? 64 : pow3_cap * 2;
        size_t need = (size_t)(n - 1) + 1;
        if (need > new_cap)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
        pow3 = tmp;
        pow3_cap = new_cap;
    }

    /* Initialise power table if empty. */
    if (pow3_len == 0) {
        pow3[0] = (seq_t)1;          /* 3^0 */
        pow3_len = 1;
    }

    /* Compute powers of 3 up to 3^(n-1). */
    for (idx_t i = pow3_len; i <= n - 1; ++i) {
        pow3[i] = pow3[i - 1] * (seq_t)3;
    }
    pow3_len = n;                    /* now pow3[0..n-1] are valid */

    /* --- Final formula: 2 * 3^(n-1) * n! -------------------------------- */
    seq_t res = (seq_t)2 * pow3[n - 1] * fact[n];
    return res;
}
