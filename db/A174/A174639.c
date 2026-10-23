/*
 * A174639  Triangle read by rows: T(n,k) = binomial(n,k) * bell(k+1) *
 *          bell(n-k+1) - bell(n+1) + 1 where bell(n) = A000110(n) are the
 *          Bell numbers.
 * https://oeis.org/A174639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   T(n,k) = binomial(n,k) * bell(k+1) * bell(n-k+1) - bell(n+1) + 1
 *            * a(n) returns the element obtained by reading the triangle by
 *            rows, * with offset 0
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* C/C++ compatible cast for realloc/malloc. */
#if defined(__cplusplus)
# define RECAST(type, ptr) reinterpret_cast<type>(ptr)
#else
# define RECAST(type, ptr) ((type)(ptr))
#endif

/* Bell number cache. */
static seq_t *bell = NULL;   /* Bell[0..] */
static size_t bell_len = 0;  /* next free index (== number of entries) */
static size_t bell_cap = 0;  /* allocated capacity */

/* Ensure the cache can hold indices up to 'need' (inclusive). */
static void ensure_bell_cap(size_t need)
{
    if (need < bell_cap) return;
    size_t new_cap = bell_cap ? bell_cap << 1 : 1;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = RECAST(seq_t *, realloc(bell, new_cap * sizeof(seq_t)));
    bell = tmp;
    bell_cap = new_cap;
}

/* Compute Bell numbers up to index n inclusive.
 * Uses B(0)=1 and B(n+1)=Σ_{k=0}^{n} C(n,k) B(k). */
static void compute_bell_up_to(size_t n)
{
    if (n < bell_len) return;
    ensure_bell_cap(n + 1);

    if (bell_len == 0) {
        bell[0] = 1;              /* B(0)=1 */
        bell_len = 1;
    }

    for (size_t i = bell_len; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            seq_t t = binomial((idx_t)i - 1, (idx_t)k);
            t *= bell[k];
            sum += t;
        }
        bell[i] = sum;
    }
    bell_len = n + 1;
}

/* Integer floor square‑root of an unsigned 64‑bit integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid && mid > n / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Convert flat index n into (row r, column k). */
    idx_t sq = 8 * n + 1;
    idx_t s = isqrt_u64(sq);
    idx_t r = (s - 1) >> 1;           /* r = floor((sqrt(8n+1)-1)/2) */

    /* Correct potential rounding errors. */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Ensure Bell numbers up to r+1 are available. */
    compute_bell_up_to((size_t)(r + 1));

    seq_t bin = binomial(r, k);

    /* Compute T(r,k) using seq_t arithmetic. */
    seq_t result = bin * bell[k + 1];
    result *= bell[r - k + 1];
    result -= bell[r + 1];
    result += 1;

    return result;
}
