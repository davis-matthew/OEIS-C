/*
 * A169191  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^27 = I.
 * https://oeis.org/A169191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * for n = 0 a(0) = 1 * for 1 ≤ n ≤ 26 a(n) = 32 *
 *            Σ_{k=1}^{n} a(n-k) + 2 * for n = 27 a(27) = 32 * Σ_{k=1}^{26}
 *            a(27-k) – 528 * a(0) + 1 * for n ≥ 28 a(n) = 32 * Σ_{k=1}^{26}
 *            a(n-k) – 528 * a(n-27) * * This is the recurrence for the number
 *            of reduced words of length n in the * Coxeter grou...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache       = NULL;   /* array of computed values      */
static idx_t  computed    = 0;      /* number of entries computed    */
static size_t allocated   = 0;      /* current array capacity        */

/* Ensure that the cache can hold indices up to n (inclusive).  The
 * allocation uses a doubling strategy to keep the operations O(1)
 * amortised.  The cast of realloc's return value is required for
 * C++ when seq_t is a class type. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= allocated) return;

    size_t new_cap = allocated ? allocated * 2 : 1;
    if ((size_t)(n + 1) > new_cap) new_cap = (size_t)(n + 1);

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* memory allocation failure */

    cache = tmp;
    allocated = new_cap;
}

/* Public accessor for the sequence.  Calls may be in any order. */
seq_t a(idx_t n)
{
    /* Return cached value if it has already been computed. */
    if (n < computed) return cache[n];

    /* Grow cache to include the requested index. */
    ensure_capacity(n);

    /* Compute values from the first missing term up to n. */
    for (idx_t i = computed; i <= n; ++i)
    {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;

        /* Add 32 * Σ_{k=1}^{min(26,i)} a(i-k) */
        idx_t limit = (i < 26 ? i : 26);
        for (idx_t k = 1; k <= limit; ++k) {
            sum += cache[i - k] * (seq_t)32;
        }

        /* Subtract 528 * a(i-27) when i ≥ 27 */
        if (i >= 27) {
            sum -= cache[i - 27] * (seq_t)528;
        }

        /* Add the constant term depending on i */
        if (i == 0) {
            sum += (seq_t)1;
        } else if (i <= 26) {
            sum += (seq_t)2;
        } else if (i == 27) {
            sum += (seq_t)1;
        }

        cache[i] = sum;
    }

    computed = n + 1;
    return cache[n];
}
