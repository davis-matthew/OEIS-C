/*
 * A025372  Numbers that are the sum of 4 nonzero squares in 7 or more ways.
 * https://oeis.org/A025372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer that can be written as the sum of four
 *            non–zero squares in at least seven unordered ways (found by
 *            enumeration of all a^2+b^2+c^2+d^2 with 1≤a≤b≤c≤d )
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation precomputes all such numbers up to a fixed
    limit (here 500 000) when first called.  The limit is chosen to
    guarantee at least 10 000 terms and the enumeration stays
    well below the 2 s running time for n≤10 000.  No external
    libraries are required.  The code compiles in both C 11 and
    C++17; malloc / calloc returns void* so explicit casts are used. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>  /* size_t */

/*  The helper below returns ⌊√n⌋.  It works for any unsigned integer
    type that fits in idx_t.  (idx_t is supplied by the build.) */
static idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n)
            return mid;
        else if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/*  Pre‑computation constants ------------------------------------------------ */
#define LIMIT 500000   /* maximum value to search; guarantees >10 000 terms */

/*  Static storage for memoization ------------------------------------------ */
static idx_t *terms     = NULL;   /* array of qualifying numbers */
static size_t terms_len = 0;      /* number of stored terms */

/*  Compute all numbers up to LIMIT that have at least 7 unordered
    representations as a sum of four non‑zero squares.  The algorithm
    enumerates 1 ≤ a ≤ b ≤ c ≤ d with early bounding to keep the
    cubic search practical.  It stores the results in 'terms'.  */
static void compute_sequence(void) {
    const idx_t max_root = isqrt_u64(LIMIT);

    /* Temporary array of squares */
    idx_t *sq = (idx_t *)malloc((max_root + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= max_root; ++i)
        sq[i] = i * i;

    /* Count how many unordered quadruples produce each sum (capped at 7). */
    unsigned char *cnt = (unsigned char *)calloc(LIMIT + 1, sizeof(unsigned char));

    for (idx_t a = 1; a <= max_root; ++a) {
        idx_t a2 = sq[a];
        for (idx_t b = a; b <= max_root; ++b) {
            idx_t ab = a2 + sq[b];
            /* minimal sum if c=d=b : ab + 2*b^2 */
            if (ab + 2 * sq[b] > LIMIT) break;
            for (idx_t c = b; c <= max_root; ++c) {
                idx_t abc = ab + sq[c];
                /* minimal sum if d=c : abc + c^2 */
                if (abc + sq[c] > LIMIT) break;
                idx_t rem = LIMIT - abc;
                idx_t dmax = isqrt_u64(rem);
                if (dmax < c) continue;
                for (idx_t d = c; d <= dmax; ++d) {
                    idx_t val = abc + sq[d];
                    if (cnt[val] < 7) ++cnt[val];
                }
            }
        }
    }
    free(sq);

    /* Collect qualifying numbers into the static array.  We allocate
       space for LIMIT+1 elements; the actual count will be far smaller. */
    terms = (idx_t *)malloc((LIMIT + 1) * sizeof(idx_t));
    terms_len = 0;
    for (idx_t i = 1; i <= LIMIT; ++i)
        if (cnt[i] >= 7)
            terms[terms_len++] = i;

    free(cnt);
}

/*  Public function required by the build system -------------------------------- */
seq_t a(idx_t n) {
    /* Compute the sequence lazily on the first call. */
    if (terms_len == 0)
        compute_sequence();

    if (n == 0 || n > (idx_t)terms_len)
        return (seq_t)0;          /* out of bounds; caller may expect 0 */

    return (seq_t)terms[(size_t)(n - 1)];
}
