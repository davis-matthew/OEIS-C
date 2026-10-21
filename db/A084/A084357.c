/*
 * A084357  Number of sets of sets of lists.
 * https://oeis.org/A084357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (n! / k!) * binomial(n-1, k-1) * Bell(k)
 *            Where Bell(k) are the Bell numbers
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

#ifdef __cplusplus
#include <vector>
#endif

/* Auxiliary: compute n! as a seq_t.  */
static seq_t factorial(idx_t n) {
    seq_t f = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        f *= (seq_t)i;
    return f;
}

/* Store Bell numbers.  */
#ifdef __cplusplus
static std::vector<seq_t> bell;

/* Ensure that bell[0..n] are computed.  */
static void ensure_bell(idx_t n) {
    if (bell.size() > n) return;
    if (bell.empty())
        bell.push_back((seq_t)1);           /* B0 = 1 */
    for (size_t k = bell.size(); k <= (size_t)n; ++k) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < k; ++j)
            sum += binom((idx_t)(k - 1), (idx_t)j) * bell[j];
        bell.push_back(sum);
    }
}
#else
static seq_t *bell = NULL;
static size_t bell_len = 0, bell_cap = 0;

/* Ensure that bell[0..n] are computed.  */
static void ensure_bell(idx_t n) {
    if (bell_len > n) return;
    size_t new_cap = bell_cap ? bell_cap : 4;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;
    if (new_cap > bell_cap) {
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        bell = tmp;
        bell_cap = new_cap;
    }
    if (bell_len == 0) {
        bell[0] = (seq_t)1;     /* B0 = 1 */
        bell_len = 1;
    }
    for (size_t k = bell_len; k <= (size_t)n; ++k) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < k; ++j)
            sum += binom((idx_t)(k - 1), (idx_t)j) * bell[j];
        bell[k] = sum;
    }
    bell_len = n + 1;
}
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;     /* a(0) = 1 */

    /* Ensure Bell numbers up to n are available.  */
    ensure_bell(n);

    /* Compute n! once, used as the initial Lah number L(n,1).  */
    seq_t fact_n = factorial(n);

    seq_t sum = (seq_t)0;
    seq_t val = fact_n;               /* L(n,1)  */
    sum += val * bell[1];             /* term k = 1 */

    for (idx_t k = 2; k <= n; ++k) {
        /* Update Lah number using the recurrence:
           L(n,k) = L(n,k-1) * (n-k+1) / (k*(k-1)).  */
        val = val * (seq_t)(n - k + 1);
        val /= (seq_t)(k * (k - 1));
        sum += val * bell[k];
    }

    return sum;
}
