/*
 * A178534  Triangle T(n,k) read by rows. T(n,1) = A000045(n+1), k > 1: T(n,k)
 *          = (Sum_{i=1..k-1} T(n-i,k-1)) - (Sum_{i=1..k-1} T(n-i,k)).
 * https://oeis.org/A178534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 276, u32 n <= 1081, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) read by rows
 * T(n,1) = A000045(n+1);   (Fibonacci, F(1)=1,F(2)=1)
 * For k>1:
 *   T(n,k) = sum_{i=1..k-1} T(n-i,k-1) - sum_{i=1..k-1} T(n-i,k)
 *
 * a(n) is the concatenation of rows of T, read row‑by‑row.
 * Example: 1; 2,1; 3,1,1; 5,2,1,1; …   →   1,2,1,3,1,1,5,...
 *
 * The function below memoises the triangle values so that repeated
 * calls do not recompute earlier rows.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square‑root (floor) using binary search on uint64_t */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* Main entry function – required signature */
seq_t a(idx_t n)
{
    static seq_t **triangle = NULL;   /* triangle rows, 1‑based */
    static size_t max_row   = 0;      /* highest row currently computed */
    static seq_t *fib = NULL;        /* Fibonacci values, fib[0] = 0, fib[1] = 1 */
    static size_t fib_len   = 0;      /* number of fib entries stored */

    if (n == 0) return (seq_t)0;      /* out of bounds – undefined */

    /* Find the row that contains position n */
    uint64_t tmp = 8ULL * n + 1;
    uint64_t s   = isqrt_uint64(tmp);
    uint64_t r   = (s - 1) >> 1;          /* floor solution */
    while (( (__uint128_t)r * (r + 1) / 2 ) < n) r++;  /* adjust upwards */

    /* Ensure sufficient rows are computed */
    if (r > max_row) {
        size_t old = max_row;
        max_row   = r;
        triangle = (seq_t**)realloc(triangle, (max_row + 1) * sizeof(seq_t*));
        /* Compute Fibonacci up to index r+1 if needed */
        if (fib_len < r + 2) {
            size_t old_fib = fib_len;
            fib_len = r + 2;
            fib = (seq_t*)realloc(fib, fib_len * sizeof(seq_t));
            if (old_fib == 0) {
                fib[0] = (seq_t)0;
                fib[1] = (seq_t)1;
                old_fib = 2;
            }
            for (size_t i = old_fib; i <= r + 1; ++i) {
                fib[i] = fib[i-1] + fib[i-2];
            }
        }
        /* Build new rows one by one */
        for (size_t row = old + 1; row <= max_row; ++row) {
            triangle[row] = (seq_t*)calloc(row + 1, sizeof(seq_t));
            /* First column */
            triangle[row][1] = fib[row + 1];
            /* Remaining columns */
            for (size_t k = 2; k <= row; ++k) {
                seq_t s1 = 0, s2 = 0;
                for (size_t i = 1; i < k; ++i) {
                    size_t m = row - i;
                    if (m >= k-1) s1 += triangle[m][k-1];
                    if (m >= k)   s2 += triangle[m][k];
                }
                triangle[row][k] = s1 - s2;
            }
        }
    }

    /* Find column index within the row */
    uint64_t prev_count = ( (__uint128_t)(r - 1) * r ) / 2;
    uint64_t col = n - prev_count;        /* 1‑based column number */

    return triangle[(size_t)r][(size_t)col];
}
