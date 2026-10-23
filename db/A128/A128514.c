/*
 * A128514  Triangle, Pell sequence in every column.
 * https://oeis.org/A128514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = P_{r - k + 1} where P is the Pell sequence * (P_1 = 1,
 *            P_m = 2*P_{m-1} + P_{m-2}), k = n - r(r-1)/2, * r = minimum
 *            integer with r(r+1)/2 >= n. The triangle is * constructed by
 *            writing the Pell numbers in reverse order * in each row.
 *            Example: 1; 2,1; 5,2,1; 12,5,2,1;
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Compute integer square root floor(sqrt(x)) for unsigned 64 bit arguments. */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);         /* hi >= sqrt(2^64-1) */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined, but safe guard */

    /* Find the row r such that r(r+1)/2 >= n (binary search). */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Previous triangular number sum: (r-1)*r/2. */
    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;

    /* Index within the row.  1 <= k <= r. */
    idx_t k = n - prev;

    /* Column index from the right: t = r - k + 1.  1 <= t <= r. */
    idx_t t = r - k + 1;

#ifdef __cplusplus
    /* C++ implementation using std::vector for dynamic growth. */
    static std::vector<seq_t> pell;
    if (pell.size() < 2) {
        pell.resize(2);
        pell[0] = (seq_t)0;
        pell[1] = (seq_t)1;
    }
    if (pell.size() <= r) {
        size_t old = pell.size();
        pell.resize(r + 1);
        for (size_t i = old; i <= (size_t)r; ++i) {
            pell[i] = ((seq_t)2) * pell[i - 1] + pell[i - 2];
        }
    }
    return pell[(size_t)t];
#else
    /* C implementation using malloc/realloc. */
    static seq_t *pell = NULL;
    static size_t pell_sz = 0;
    if (pell_sz <= r) {
        size_t new_sz = (size_t)r + 1;
        if (pell == NULL) {
            pell = (seq_t*)malloc(new_sz * sizeof(seq_t));
            pell_sz = 0;
        } else {
            pell = (seq_t*)realloc(pell, new_sz * sizeof(seq_t));
        }
        if (pell_sz == 0) {
            pell[0] = (seq_t)0;
            pell_sz = 1;
        }
        if (pell_sz == 1) {
            pell[1] = (seq_t)1;
            pell_sz = 2;
        }
        for (size_t i = pell_sz; i <= (size_t)r; ++i) {
            pell[i] = ((seq_t)2) * pell[i - 1] + pell[i - 2];
        }
        pell_sz = new_sz;
    }
    return pell[(size_t)t];
#endif
}
