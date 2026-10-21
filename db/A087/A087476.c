/*
 * A087476  Triangle read by rows where the n-th row has n terms:
 *          T(n,i)=(i+2)^2-4 mod n.
 * https://oeis.org/A087476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k^2 + 4k) mod r, where r is the smallest integer with
 *            r*(r+1)/2 ≥ n (row index) and k = n - (r-1)*r/2 (column index)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /* Find r such that r*(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrt((double)(8*n + 1)) - 1) / 2);
    /* Correct possible off‑by‑one due to floating point */
    while (r * (r + 1) / 2 < n) ++r;
    while (r > 1 && (r - 1) * r / 2 >= n) --r;

    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    seq_t ks = (seq_t)k;
    seq_t rs = (seq_t)r;

    return ((ks * ks + (seq_t)4 * ks) % rs);
}
