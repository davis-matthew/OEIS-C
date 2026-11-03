/*
 * A069134  (n!*(3*n))^2.
 * https://oeis.org/A069134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!*(3*n))^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *fact = NULL;
    static idx_t fact_max = 0;   /* largest n for which fact[n] is valid */
    static size_t fact_cap = 0;  /* allocated capacity */

    if (fact == NULL) {
        fact_cap = 1;
        fact = mem_new(seq_t, fact_cap);
        if (!fact) return 0;          /* out‑of‑memory fallback */
        fact[0] = (seq_t)1;           /* 0! = 1 */
    }

    if (n > fact_max) {
        /* grow capacity to at least n+1 elements */
        size_t new_cap = fact_cap;
        if (new_cap == 0) new_cap = 1;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1;

        if (new_cap > fact_cap) {
            fact = mem_resize(seq_t, fact, new_cap);
            fact_cap = new_cap;
        }

        /* compute factorials from fact_max+1 to n */
        for (idx_t i = fact_max + 1; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_max = n;
    }

    seq_t mult = (seq_t)3 * (seq_t)n;      /* 3*n */
    seq_t val  = fact[n] * mult;           /* n! * 3*n */
    return val * val;                      /* square it */
}
