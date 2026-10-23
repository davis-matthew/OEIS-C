/*
 * A121348  Difference between (n!)^2 and the next smaller factorial.
 * https://oeis.org/A121348
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! * n!) - k! where k! is the greatest factorial < (n! *
 *            n!)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fac_cache = NULL;   /* factorial cache, fac_cache[i] = i! */
static idx_t fac_count  = 0;      /* highest index for which fac_cache[i] is defined */

/* Ensure fac_cache contains factorials up to at least n */
static void _ensure_factorial_up_to(idx_t n)
{
    if (fac_cache == NULL) {
        size_t len = (size_t)(n + 1);
        fac_cache = (seq_t *)malloc(len * sizeof(seq_t));
        if (!fac_cache) return;          /* allocation failed; caller will use whatever is available */
        fac_cache[0] = 1;
        fac_count = 0;
        if (n >= 1) {
            fac_cache[1] = 1;           /* 1! = 1 */
            fac_count = 1;
        }
    }

    if (fac_count >= n) return;

    size_t old_len = (size_t)(fac_count + 1);
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(fac_cache, new_len * sizeof(seq_t));
    if (!tmp) return;                  /* allocation failed; keep old cache */
    fac_cache = tmp;

    for (idx_t i = fac_count + 1; i <= n; ++i)
        fac_cache[i] = fac_cache[i - 1] * (seq_t)i;

    fac_count = n;
}

/* Extend fac_cache until fac_cache[fac_count] >= target */
static void _extend_factorial_until(seq_t target)
{
    while (fac_cache[fac_count] < target) {
        idx_t next = fac_count + 1;
        seq_t *tmp = (seq_t *)realloc(fac_cache, (size_t)(next + 1) * sizeof(seq_t));
        if (!tmp) return;              /* allocation failed; keep current cache */
        fac_cache = tmp;
        fac_cache[next] = fac_cache[next - 1] * (seq_t)next;
        fac_count = next;
    }
}

seq_t a(idx_t n)
{
    /* For n < 2 the definition is not required; return 0 */
    if (n < 2) return (seq_t)0;

    /* Compute factorial up to n at least once */
    _ensure_factorial_up_to(n);

    seq_t f_n = fac_cache[n];
    seq_t square = f_n * f_n;

    /* Ensure we have factorials up to the first one >= square */
    if (square > fac_cache[fac_count])
        _extend_factorial_until(square);

    /* Binary search the largest k with fac_cache[k] < square */
    idx_t low = 0;
    idx_t high = fac_count;
    idx_t k = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (fac_cache[mid] < square) {
            k = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    return square - fac_cache[k];
}
