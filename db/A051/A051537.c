/*
 * A051537  Triangle read by rows: T(i,j) = lcm(i,j)/gcd(i,j) for 1 <= j <= i.
 * https://oeis.org/A051537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i*j / gcd(i,j)^2, where (i,j) are the row and column
 *            indices such that n = i(i−1)/2 + j (rows and columns start at 1)
 * Range:     u8 n <= 151, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euclidean algorithm for idx_t (uint64_t). */
static inline idx_t my_gcd(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute the n-th term of A051537. */
seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 >= n and (r-1)r/2 < n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                          /* row index */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)prev;              /* column index */

    /* Compute T(r,j) = r*j / gcd(r,j)^2. */
    idx_t g = my_gcd(r, j);
    seq_t num = (seq_t)r * (seq_t)j;
    seq_t denom = (seq_t)g * (seq_t)g;
    return num / denom;
}
