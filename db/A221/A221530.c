/*
 * A221530  Triangle read by rows: T(n,k) = A000005(k)*A000041(n-k).
 * https://oeis.org/A221530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 862, u32 n <= 7384, u64 n <= 8927,
 *            u128 n <= 8927, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the nth entry of the triangle read by rows, where
 * the triangle entries are
 *     T(n,k) = A000005(k) * A000041(n-k)   (numbers of divisors of k
 *     times the partition number of n-k)
 * With offset 1.  For a given 1‑based index n, find the row r and
 * column c such that 
 *     (r-1)*r/2 < n ≤ r*(r+1)/2,
 * then return T(r,c).
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

static idx_t _precomp_limit = 0;         /* largest r that has been precomputed */
static seq_t * _partitions = NULL;       /* partition numbers p[0..r] */
static unsigned * _divisors  = NULL;     /* divisor counts d[0..r] */

static void _ensure_precomp(idx_t r)
{
    if (r <= _precomp_limit) return;    /* already ready */

    /* allocate new arrays */
    seq_t * new_parts;
    unsigned * new_divs;
#if defined(__cplusplus)
    new_parts = new seq_t[r+1];
    new_divs = new unsigned[r+1];
#else
    new_parts = (seq_t*)malloc((size_t)(r+1) * sizeof(seq_t));
    new_divs = (unsigned*)malloc((size_t)(r+1) * sizeof(unsigned));
    if (!new_parts || !new_divs) return;   /* allocation failure, unlikely */
#endif

    /* initialise to 0 */
    for (idx_t i = 0; i <= r; ++i) {
        new_parts[i] = 0;
        new_divs[i]  = 0;
    }
    new_parts[0] = 1;   /* p(0) = 1 */

    /* compute divisor counts via simple sieve */
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = i; j <= r; j += i) {
            new_divs[j] += 1;
        }
    }

    /* compute partition numbers with DP: p[n] = p[n-1]+p[n-2]-... */
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = i; j <= r; ++j) {
            new_parts[j] += new_parts[j - i];
        }
    }

    /* replace old arrays */
#if defined(__cplusplus)
    delete[] _partitions;
    delete[] _divisors;
#else
    free(_partitions);
    free(_divisors);
#endif
    _partitions = new_parts;
    _divisors   = new_divs;
    _precomp_limit = r;
}

static idx_t _row_of_n(idx_t n)
{
    /* Find smallest r such that r*(r+1)/2 >= n */
    idx_t sum = 0;
    idx_t r = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }
    return r;
}

seq_t a(idx_t n) /* n >= 1 */
{
    /* Determine row r and column c in the triangular arrangement */
    idx_t r = _row_of_n(n);
    idx_t prev_tri = (r * (r - 1)) / 2;           /* (r-1)*r/2 */
    idx_t c = n - prev_tri;                       /* 1 <= c <= r */

    /* Ensure we have partitions and divisor counts up to r */
    _ensure_precomp(r);

    seq_t part = _partitions[r - c];              /* p(r-c) */
    seq_t divs = (seq_t)_divisors[c];            /* d(c) */

    return divs * part;
}
