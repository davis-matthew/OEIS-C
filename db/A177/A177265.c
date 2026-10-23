/*
 * A177265  Number of permutations of {1,2,...,n} having exactly one string of
 *          consecutive fixed points (including singletons).
 * https://oeis.org/A177265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + n*A000166(n-1), a(1)=1 * where A000166(j) are
 *            the derangement numbers: * D(0)=1, D(1)=0, D(k) =
 *            (k-1)*(D(k-1)+D(k-2)) for k≥2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static idx_t _max_n = 0;
static seq_t *_der   = NULL;   /* derangements D(k) for k=0.._max_n   */
static size_t _der_cap = 0;
static seq_t *_a     = NULL;   /* sequence values a(k) for k=0.._max_n */
static size_t _a_cap  = 0;

/* Ensure all values up to n are computed and cached. */
static void _ensure(idx_t n)
{
    if (n <= _max_n)
        return;

    /* Desired capacity: indices 0..n inclusive */
    size_t need = (size_t)(n + 1);

    /* Allocate or grow derangement array */
    if (_der == NULL) {
        _der = seq_alloc(need);
        _der_cap = need;
        if (_der == NULL) return;          /* allocation failed */
        _der[0] = (seq_t)1;
        _der[1] = (seq_t)0;
    } else if (_der_cap < need) {
        seq_t *tmp = seq_realloc(_der, _der_cap, need);
        if (tmp == NULL) return;
        _der = tmp;
        _der_cap = need;
    }

    /* Allocate or grow sequence array */
    if (_a == NULL) {
        _a = seq_alloc(need);
        _a_cap = need;
        if (_a == NULL) return;          /* allocation failed */
        _a[0] = (seq_t)0;
    } else if (_a_cap < need) {
        seq_t *tmp = seq_realloc(_a, _a_cap, need);
        if (tmp == NULL) return;
        _a = tmp;
        _a_cap = need;
    }

    /* Compute missing entries */
    for (idx_t k = _max_n + 1; k <= n; ++k) {
        if (k >= 2) {
            seq_t sum = _der[(size_t)(k - 1)] + _der[(size_t)(k - 2)];
            _der[(size_t)k] = ((seq_t)(k - 1)) * sum;
        } /* else _der[0] and _der[1] are already set */

        /* a(k) = a(k-1) + k * D(k-1) */
        seq_t prod = ((seq_t)k) * _der[(size_t)(k - 1)];
        _a[(size_t)k] = _a[(size_t)(k - 1)] + prod;
    }

    _max_n = n;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* out of range, harmless for the OEIS offset */
    _ensure(n);
    return _a[(size_t)n];
}
