/*
 * A011967  4th differences of Bell numbers.
 * https://oeis.org/A011967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B_{n+4} - 4 * B_{n+3} + 6 * B_{n+2} - 4 * B_{n+1} + B_n
 *            where B_k are the Bell numbers defined by B_0 = 1 and B_{k+1} =
 *            Σ_{i=0}^k C(k,i) * B_i. This implements a(n) = Δ⁴ B_n (fourth
 *            forward difference of Bell numbers)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *bell_cache = NULL;
static size_t bell_computed = 0;

/* Ensure Bell numbers up to idx (inclusive) are computed and stored in bell_cache. */
static void ensure_bell(size_t idx)
{
    if (bell_cache == NULL) {
        /* Allocate enough space for indices 0..idx */
        bell_cache = (seq_t *)malloc((idx + 1) * sizeof(seq_t));
        if (bell_cache == NULL) return;      /* allocation failure: silently abort */
        /* Bell[0] = 1 */
        bell_cache[0] = (seq_t)1;
        bell_computed = 1;
    } else if (bell_computed > idx) {
        return; /* already computed */
    } else {
        /* Grow the allocated array to fit up to idx */
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(bell_cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return; /* allocation failure */
        bell_cache = tmp;
    }

    /* Compute missing Bell numbers from bell_computed up to idx */
    for (size_t n = bell_computed; n <= idx; ++n) {
        if (n == 0) {
            bell_cache[0] = (seq_t)1;
            continue;
        }
        size_t m = n - 1;
        seq_t sum = 0;
        seq_t binom = 1;          /* C(m,0) */
        for (size_t k = 0; k <= m; ++k) {
            sum += binom * bell_cache[k];
            if (k < m) {
                /* binom = binom * (m - k) / (k + 1)    */
                binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
            }
        }
        bell_cache[n] = sum;
    }
    bell_computed = idx + 1;
}

seq_t a(idx_t n)
{
    /* Need Bell numbers up to n+4 */
    size_t target = (size_t)n + 4;
    ensure_bell(target);

    /* Compute the fourth forward difference */
    seq_t result = bell_cache[n + 4];
    result -= 4 * bell_cache[n + 3];
    result += 6 * bell_cache[n + 2];
    result -= 4 * bell_cache[n + 1];
    result += bell_cache[n];
    return result;
}
