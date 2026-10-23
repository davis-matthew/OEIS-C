/*
 * A107981  Triangle read by rows: T(n,k) = (k+1)(k+2)(n+2)(n+3)(6n^2 - 8n*k +
 *          18n + 3k^2 - 11k + 12)/144 for 0<=k<=n.
 * https://oeis.org/A107981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where row = ⌊(√(8n+1)-1)/2⌋, col = n -
 *            row*(row+1)/2, and T(n,k) =
 *            (k+1)(k+2)(n+2)(n+3)(6n²-8nk+18n+3k²-11k+12)/144
 * Range:     u8 n <= 0, u16 n <= 13, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: find largest r such that r*(r+1)/2 <= n */
static idx_t row_of_index(idx_t n) {
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        __int128 prod = (__int128)mid * (mid + 1);
        if (prod <= (__int128)2 * n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t row = row_of_index(n);
    /* count_before = row*(row+1)/2, use 128‑bit for safety */
    __int128 before = (__int128)row * (row + 1) / 2;
    idx_t col = n - (idx_t)before;

    /* cast to seq_t for arithmetic */
    seq_t N = (seq_t)row;
    seq_t K = (seq_t)col;

    seq_t k1 = K + 1;
    seq_t k2 = K + 2;
    seq_t n1 = N + 2;
    seq_t n2 = N + 3;

    seq_t inner = 6 * N * N - 8 * N * K + 18 * N
                + 3 * K * K - 11 * K + 12;

    seq_t prod = k1 * k2 * n1 * n2 * inner;
    return prod / 144;
}
