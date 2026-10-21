/*
 * A219859  Triangular array read by rows: T(n,k) is the number of
 *          endofunctions, functions f:{1,2,...,n}->{1,2,...,n}, that have
 *          exactly k elements with no preimage; n>=0, 0<=k<=n.
 * https://oeis.org/A219859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = largest integer with r*(r+1)/2 <= n, k =
 *            n - r*(r+1)/2, T(r,k) = r!/k! * S(r, r-k) (S = Stirling numbers
 *            of the second kind)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* for bool, true, false */

#define MAX_R 2000          /* sufficient for the problem limits */

static seq_t stirling_table[MAX_R + 1][MAX_R + 1];
static bool   stirling_initialized = false;

static void init_stirling(void)
{
    if (stirling_initialized) return;
    stirling_initialized = true;

    for (idx_t n = 0; n <= MAX_R; ++n) {
        for (idx_t k = 0; k <= MAX_R; ++k) {
            if (n == 0 && k == 0)
                stirling_table[n][k] = 1;
            else if (k == 0 || k > n)
                stirling_table[n][k] = 0;
            else {
                seq_t term1 = stirling_table[n - 1][k - 1];
                seq_t term2 = (seq_t)k * stirling_table[n - 1][k];
                stirling_table[n][k] = term1 + term2;
            }
        }
    }
}

/* Return the largest r such that r*(r+1)/2 <= n */
static idx_t row_of_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n + 1;          /* r is always < n+1 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {          /* 0*(0+1)/2 = 0 <= n always */
            low = mid + 1;
            continue;
        }
        /* compute mid*(mid+1)/2 without overflow */
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    init_stirling();

    idx_t r = row_of_index(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* falling factorial r!/k! = product of integers from k+1 to r */
    seq_t falling = 1;
    for (idx_t j = k + 1; j <= r; ++j)
        falling *= j;

    seq_t stir = stirling_table[r][r - k];
    return falling * stir;
}
