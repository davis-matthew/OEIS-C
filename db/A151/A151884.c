/*
 * A151884  Let g be a permutation of [1..n] having say j_i cycles of length
 *          i, with Sum_i i*j_i = n; sequence gives Sum_g Sum_{i odd} (j_i)^2.
 * https://oeis.org/A151884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ( Σ_{odd i ≤ n} 1/i + Σ_{odd i ≤ n/2} 1/i² )
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *factorials = NULL;
static idx_t max_fact  = 0;

/* Compute OEIS sequence A151884 for index n (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined case – safeguard */

    /* Grow factorial table if necessary. */
    if (n > max_fact) {
        size_t new_size = (size_t)(n + 1);   /* include factorial[0] */
        if (!factorials) {
            factorials = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!factorials)
                return (seq_t)0;           /* allocation failure */
            factorials[0] = (seq_t)1;
        } else {
            seq_t *tmp = (seq_t *)realloc(factorials, new_size * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;           /* reallocation failure */
            factorials = tmp;
        }
        for (idx_t i = max_fact + 1; i <= n; ++i)
            factorials[i] = factorials[i-1] * (seq_t)i;
        max_fact = n;
    }

    seq_t fact_n = factorials[n];
    seq_t result = (seq_t)0;

    for (idx_t i = 1; i <= n; i += 2) {
        result += fact_n / (seq_t)i;          /* n! / i */
        if (i <= n / 2) {
            uint64_t ii = (uint64_t)i * (uint64_t)i;   /* i², fits in 64‑bit */
            result += fact_n / (seq_t)ii;                /* n! / i² */
        }
    }
    return result;
}
