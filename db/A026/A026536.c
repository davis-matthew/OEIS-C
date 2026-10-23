/*
 * A026536  Irregular triangular array T read by rows: T(i,0 ) = T(i,2i) = 1
 *          for i >= 0; T(i,1) = T(i,2i-1) = floor(i/2) for i >= 1; for even n
 *          >= 2, T(i,j) = T(i-1,j-2) + T(i-1,j-1) + T(i-1,j) for j = 2..2i-2,
 *          for odd n >= 3, T(i,j) = T(i-1,j-2) + T(i-1,j) for j = 2..2i-2.
 * https://oeis.org/A026536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the T(r,c) from the irregular triangular array * T
 *            defined by * T(i,0)=T(i,2i)=1, * T(i,1)=T(i,2i-1)=floor(i/2)
 *            (i>=1), * for i even >=2: T(i,j)=T(i-1,j-2)+T(i-1,j-1)+T(i-1,j),
 *            * for i odd >=3: T(i,j)=T(i-1,j-2)+T(i-1,j) (j=2..2i-2) * Then
 *            a(n)=T(floor(sqrt(n)), n - (floor(sqrt(n)))^2)
 * Range:     u8 n <= 87, u16 n <= 266, u32 n <= 807, u64 n <= 2859,
 *            u128 n <= 9816, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
static inline seq_t *alloc_row(idx_t len)
{
    return new seq_t[len];
}
static inline void free_row(seq_t *p)
{
    delete[] p;
}
#else
static inline seq_t *alloc_row(idx_t len)
{
    return (seq_t*)malloc(len * sizeof(seq_t));
}
static inline void free_row(seq_t *p)
{
    free(p);
}
#endif

/* integer square root: floor(sqrt(n)) */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            lo = 1;
            continue;
        }
        if (mid > n / mid) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    idx_t row = int_sqrt_u64(n);
    idx_t col = n - row * row;

    seq_t *prev = NULL;

    for (idx_t i = 0; i <= row; ++i) {
        idx_t cur_len = 2 * i + 1;
        seq_t *cur = alloc_row(cur_len);
        if (!cur) {
            free_row(prev);
            return (seq_t)0;
        }

        cur[0] = (seq_t)1;
        cur[2 * i] = (seq_t)1;
        if (i >= 1) {
            seq_t half = (seq_t)(i / 2);
            cur[1] = half;
            cur[2 * i - 1] = half;
        }

        if (i >= 2) {
            if ((i & 1) == 0) { /* even i */
                for (idx_t j = 2; j <= 2 * i - 2; ++j) {
                    cur[j] = prev[j - 2] + prev[j - 1] + prev[j];
                }
            } else { /* odd i */
                for (idx_t j = 2; j <= 2 * i - 2; ++j) {
                    cur[j] = prev[j - 2] + prev[j];
                }
            }
        }

        if (i == row) {
            seq_t result = cur[col];
            free_row(prev);
            free_row(cur);
            return result;
        }

        free_row(prev);
        prev = cur;
    }

    return (seq_t)0;
}
