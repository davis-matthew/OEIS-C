/*
 * A163842  Triangle interpolating the swinging factorial (A056040) restricted
 *          to odd indices with its binomial transform. Same as interpolating
 *          the beta numbers 1/beta(n,n) (A002457) with (A163869). Triangle
 *          read by rows, for n >= 0, k >= 0.
 * https://oeis.org/A163842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{i=k..n} binomial(n-k,n-i)*(2i+1) where i denotes
 *            the swinging factorial of i (A056040)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 89, u64 n <= 377,
 *            u128 n <= 1274, big any n
 * Helpers:   binom, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/binom.h"

static inline idx_t _triangle_row(idx_t N) {
    /* Find maximum r such that r*(r+1)/2 <= N */
    idx_t low = 0;
    idx_t high = 1;
    while (( (__int128)high * (high + 1) / 2 ) <= N) {
        if (high > ((1ULL << 31) - 1)) break; /* guard against an infinite loop for huge N */
        high <<= 1;
    }
    low = high >> 1;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (( (__int128)mid * (mid + 1) / 2 ) <= N)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;          /* number of stored terms */

static void _ensure_a_cache(size_t needed) {
    if (needed < a_cache_size) return;
    size_t new_cap = needed + 1;
    seq_t *new_cache = seq_alloc(new_cap);
    /* copy old values if any */
    if (a_cache) {
        for (size_t i = 0; i < a_cache_size; ++i)
            new_cache[i] = a_cache[i];
    }
    /* compute new terms */
    for (size_t i = a_cache_size; i <= needed; ++i) {
        if (i == 0) {
            new_cache[0] = 1;
        } else {
            /* recurrence: a(i) = a(i-1) * (2i+1)*(2i) / (i*i)   */
            seq_t prev = new_cache[i - 1];
            seq_t num  = (seq_t)(2 * i + 1) * (seq_t)(2 * i);
            seq_t den  = (seq_t)(i) * (seq_t)(i);
            new_cache[i] = prev * num / den;
        }
    }
    a_cache = new_cache;
    a_cache_size = new_cap;
}

seq_t a(idx_t n) {
    /* Determine row r and column c in the triangular array */
    idx_t r = _triangle_row(n);
    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start;             /* 0 <= c <= r */

    /* Ensure values a[0..r] are available */
    _ensure_a_cache((size_t)r);

    /* Compute T(r,c) = sum_{j=0}^{r-c} C(r-c, j) * a[r-j] */
    idx_t width = r - c;
    seq_t result = 0;
    seq_t coeff  = 1;                     /* C(width, 0) */

    for (idx_t j = 0; j <= width; ++j) {
        size_t idx = (size_t)(r - j);
        result += coeff * a_cache[idx];
        if (j < width) {
            /* Update coeff: C(n, j+1) = C(n, j) * (n-j) / (j+1) */
            coeff = coeff * (seq_t)(width - j) / (seq_t)(j + 1);
        }
    }
    return result;
}
