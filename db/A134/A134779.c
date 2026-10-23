/*
 * A134779  Integer coefficients of A(x), where a(n) is the least integer
 *          greater than a(n-1)+2 such that A(x)^(1/2) has integer
 *          coefficients.
 * https://oeis.org/A134779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 7012, u32 n <= 7012, u64 n <= 7012,
 *            u128 n <= 7012, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1 ;
 * For n>0 let
 *      S = Σ_{i=1}^{n-1} b[i]*b[n-i]   (b[0]=1)
 *      b[n] = (a[n]-S)/2
 *      a[n] is the smallest integer ≥ a[n-1]+2 whose parity equals that of S.
 * This ensures that A(x)^(1/2)=Σ_{k≥0} b(k)x^k has integer coefficients.
 * The function memoises the sequence so that calls are fast and can be
 * made in any order. */
#include <stdlib.h>

static uint64_t *a64 = NULL;
static uint64_t *b64 = NULL;
static size_t   max_n = 0;
static size_t   capacity = 0;

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (capacity == 0) {
        capacity = 1;
        a64 = (uint64_t *)malloc(capacity * sizeof(uint64_t));
        b64 = (uint64_t *)malloc(capacity * sizeof(uint64_t));
        a64[0] = 1ULL;
        b64[0] = 1ULL;
        max_n   = 0;
    }

    /* grow storage if necessary */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity << 1;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        a64 = (uint64_t *)realloc(a64, new_cap * sizeof(uint64_t));
        b64 = (uint64_t *)realloc(b64, new_cap * sizeof(uint64_t));
        capacity = new_cap;
    }

    /* compute values up to n */
    for (idx_t k = (idx_t)(max_n + 1); k <= n; ++k) {
        uint64_t S = 0ULL;
        for (idx_t i = 1; i < k; ++i) {
            S += b64[i] * b64[k - i];
        }

        uint64_t base = a64[k - 1] + 2ULL;
        if ((S & 1ULL) != (base & 1ULL))  /* parity mismatch → add 1 */
            base += 1ULL;

        a64[k] = base;
        b64[k] = (base - S) / 2ULL;
    }

    max_n = (size_t)n;
    return (seq_t)a64[n];
}
