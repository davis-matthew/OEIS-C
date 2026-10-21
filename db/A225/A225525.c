/*
 * A225525  a(n) = (sigma(2*n) - sigma(n))*Lucas(n) where Lucas(n) =
 *          A000204(n) and sigma(n) = A000203(n) is the sum of divisors of n.
 * https://oeis.org/A225525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sigma(2*n) - sigma(n)) * Lucas(n) Lucas(0)=2,
 *            Lucas(1)=1, Lucas(n)=Lucas(n-1)+Lucas(n-2) for n>=2 sigma(n) =
 *            sum of divisors of n (A000203)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   ensure_capacity, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t n); */
#include "../utils/ensure_capacity.h"   /* void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size); */

/* static cache for Lucas numbers (1-indexed).  lucas_cache[0] is unused. */
static seq_t *lucas_cache = NULL;      /* array of Lucas numbers */
static size_t lucas_cap   = 0;         /* currently allocated capacity (in elements) */
static size_t lucas_nmax  = 0;         /* highest n for which lucas_cache[n] is valid */

/* previous known values for initialization */
static const seq_t L0 = 2; /* not used directly but kept for clarity */
static const seq_t L1 = 1;

/* Helper: extend lucas cache up to at least 'n' */
static void ensure_lucas(idx_t n)
{
    if (n <= lucas_nmax)
        return;                         /* already computed */

    /* ensure capacity: we need indices 0..n (size n+1) */
    size_t needed = (size_t)n + 1;
    ensure_capacity(needed, &lucas_cap, (void **)&lucas_cache, sizeof(seq_t));

    /* initialize on first use */
    if (lucas_nmax == 0) {
        lucas_cache[0] = L0;  /* for convenience: L0=2 */
        lucas_cache[1] = L1;  /* L1=1 */
        lucas_nmax = 1;
    }

    /* compute missing Lucas numbers */
    for (size_t i = lucas_nmax + 1; i <= (size_t)n; ++i) {
        lucas_cache[i] = lucas_cache[i - 1] + lucas_cache[i - 2];
    }
    lucas_nmax = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1)                /* indices below offset are undefined */
        return 0;

    /* compute Lucas number for n */
    ensure_lucas(n);

    /* sigma for n and 2*n */
    seq_t sigma_n  = sigma_of(n);
    seq_t sigma_2n = sigma_of(2 * n);

    seq_t diff = sigma_2n - sigma_n;           /* guaranteed non‑negative */
    seq_t result = diff * lucas_cache[(size_t)n];

    return result;
}
