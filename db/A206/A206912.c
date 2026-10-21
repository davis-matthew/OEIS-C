/*
 * A206912  Position of log(n+1) when the partial sums of the harmonic series
 *          are jointly ranked with the set {log(k+1)}; complement of A206911.
 * https://oeis.org/A206912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + #{k ≥ 1 | H_k ≤ ln(n+1)} , where H_k = 1 + 1/2 + … +
 *            1/k (harmonic number)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* static cache of harmonic numbers, stored as double for comparison.
   The array is indexed from 0 with H[0] = H_1.  */
static double *harmonic_cache = NULL;
static size_t harmonic_cap   = 0;
static size_t harmonic_len   = 0;

/* Ensure the cache holds at least up to the index needed for target > log(n+1).  */
static void ensure_harmonic_cache(double logn)
{
    if (harmonic_len == 0) {
        /* Allocate initial capacity.  */
        harmonic_cap = 1024;
        harmonic_cache = (double *)malloc(harmonic_cap * sizeof(double));
        harmonic_len   = 0;
    }

    /* Grow the cache until the last value exceeds log(n+1).  */
    double sum = harmonic_len ? harmonic_cache[harmonic_len - 1] : 0.0;
    size_t i = harmonic_len;
    while (i == 0 || harmonic_cache[i - 1] <= logn) {
        if (i == harmonic_cap) {
            harmonic_cap *= 2;
            harmonic_cache = (double *)realloc(harmonic_cache, harmonic_cap * sizeof(double));
        }
        sum += 1.0 / (double)(i + 1);  /* i currently 0-based for H_1.  */
        harmonic_cache[i] = sum;
        ++i;
    }
    harmonic_len = i;
}

/* Binary search to count how many H_k <= logn.  */
static size_t count_harmonics_le(double logn)
{
    size_t lo = 0, hi = harmonic_len;  /* [lo, hi)  */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (harmonic_cache[mid] <= logn)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;  /* number of elements <= logn */
}

seq_t a(idx_t n)
{
    /* log(n+1) as a double.  */
    double logn = log((double)(n + 1));

    /* Make sure the harmonic cache contains enough terms.  */
    ensure_harmonic_cache(logn);

    /* Count of harmonic numbers <= log(n+1).  */
    size_t cnt = count_harmonics_le(logn);

    /* a(n) = n + cnt  , both in seq_t  */
    return (seq_t)n + (seq_t)cnt;
}
