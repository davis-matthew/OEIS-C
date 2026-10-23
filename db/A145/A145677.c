/*
 * A145677  Triangle T(n, k) read by rows: T(n, 0) = 1, T(n, n) = n, n>0,
 *          T(n,k) = 0, 0 < k < n-1.
 * https://oeis.org/A145677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 if n is the first element of a row of the
 * triangle; = r if n is the last element of the row r (r > 0);
 * = 0 otherwise.
 * The triangle has rows: 1; 1,1; 1,0,2; 1,0,0,3; …
 * Row r (r≥0) has r+1 elements; its first index is r(r+1)/2.
 */

#include <math.h>
#include <stdint.h>   /* for uint64_t; idx_t is defined elsewhere */

static idx_t triangular_root(idx_t n) {
    /* largest r such that r(r+1)/2 <= n */
    long double val = sqrtl((long double)(8.0L * n + 1.0L));
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);

    /* correct possible rounding errors */
    while (((__int128)(r + 1) * (r + 2)) / 2 <= n) r++;
    while (((__int128)r * (r + 1)) / 2 > n) r--;

    return r;
}

seq_t a(idx_t n) {
    idx_t r = triangular_root(n);
    idx_t offset = (idx_t)((( __int128 )r * (r + 1)) / 2);
    idx_t c = n - offset;

    if (r == 0 || c == 0)          /* first element of a row */
        return (seq_t)1;
    if (c == r)                    /* last element of row r (r>0) */
        return (seq_t)r;
    return (seq_t)0;                /* all other positions are zero */
}
