/*
 * A116590  a(0)=1; a(n)=b(n+2)+b(n), where b(n)=A005185(n) is the Hofstadter
 *          Q-sequence: b(1)=b(2)=1; b(n)=b(n-b(n-1))+b(n-b(n-2)) for n > 2.
 * https://oeis.org/A116590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = A005185(n+2) + A005185(n) * where
 *            A005185 is the Hofstadter Q‑sequence defined by * b(1)=b(2)=1; *
 *            b(n)=b(n‑b(n‑1))+b(n‑b(n‑2)) for n>2
 * Range:     u8 n <= 232, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

/* static cache for the Hofstadter Q‑sequence */
static seq_t *b_cache = NULL;      /* array holding b(1), b(2), … */
static idx_t  b_max  = 0;          /* maximum index currently computed */
static size_t b_cap  = 0;          /* allocated capacity of b_cache */

/* ensure that b_cache contains values up to index 'up_to' inclusive */
static void ensure_b(idx_t up_to)
{
    /* allocate or expand the cache first */
    if (b_cache == NULL) {
        b_cap = (size_t)(up_to + 1);
        b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (b_cache == NULL) return;   /* allocation failed – caller will abort */
        b_max = 0;
    }
    if ((size_t)up_to + 1 > b_cap) {   /* need more space */
        b_cap = (size_t)(up_to + 1);
        b_cache = (seq_t *)realloc(b_cache, b_cap * sizeof(seq_t));
        if (b_cache == NULL) return;   /* allocation failed – caller will abort */
    }

    /* compute any missing entries */
    for (idx_t i = b_max + 1; i <= up_to; ++i) {
        if (i <= 2) {
            b_cache[i] = (seq_t)1;
        } else {
            /* indices for the recursive calls; cast % because b_cache holds seq_t */
            idx_t idx1 = (idx_t)b_cache[i - 1];
            idx_t idx2 = (idx_t)b_cache[i - 2];
            /* compute b(i) = b(i‑idx1) + b(i‑idx2) */
            seq_t val = b_cache[i - idx1] + b_cache[i - idx2];
            b_cache[i] = val;
        }
    }
    b_max = up_to;
}

/* Public function as required by the build */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;   /* a(0) = 1 */
    }
    /* we need b(n) and b(n+2) */
    idx_t need = n + 2;
    ensure_b(need);
    return b_cache[n] + b_cache[n + 2];
}
