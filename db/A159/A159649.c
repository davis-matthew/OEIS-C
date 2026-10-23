/*
 * A159649  Numerator of Hermite(n, 11/19).
 * https://oeis.org/A159649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 22*a(n‑1) - 722*(n-1)*a(n‑2), a(0)=1, a(1)=22
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t cache[20001] = { 1 };   /* cache[0] = 1, others zero‑initialized */
static idx_t max_computed = 0;       /* largest index already stored in cache */

/* Compute terms up to and including n, storing them in cache */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed)
        return;

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 22;
        } else if (i >= 2) {
            seq_t term = 22 * cache[i - 1];
            term -= 722 * ((seq_t)(i - 1) * cache[i - 2]);
            cache[i] = term;
        }
    }
    max_computed = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n <= max_computed)          /* value already cached */
        return cache[n];

    /* For n beyond the static cache size, compute on the fly */
    seq_t a0 = 1;
    if (n == 0) return a0;
    seq_t a1 = 22;
    if (n == 1) return a1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t an = 22 * a1 - 722 * ((seq_t)(i - 1) * a0);
        a0 = a1;
        a1 = an;
    }
    return a1;
}
