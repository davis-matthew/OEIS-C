/*
 * A099889  XOR difference triangle of the odd numbers, read by rows.
 * https://oeis.org/A099889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8000, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the entry in the XOR difference triangle of the odd numbers.
 * Let r be the unique integer such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,
 * and k = n - r*(r+1)/2.  Then
 *     a(n) = XOR_{i⊆k} ( 2*(r-i) + 1 ),
 * where the XOR is taken over all sub‑masks i of k (i.e., all i with
 * 0 ≤ i ≤ k and i & (k-i) == 0).
 */

#include <stdint.h>
#include <stdbool.h>

/* Find the row index r for a given flattened index n.  */
static inline idx_t
row_of_n(idx_t n)
{
    idx_t r = 0;
    /* Increment r until the next row would exceed n.  */
    while (( (__int128)(r + 1) * (r + 2) / 2 ) <= n)
        ++r;
    return r;
}

seq_t
a(idx_t n)
{
    idx_t r = row_of_n(n);
    idx_t k = n - r * (r + 1) / 2;

    seq_t xor_acc = 0;
    idx_t sub = k;
    do {
        seq_t val = 2 * ((seq_t)(r - sub)) + 1;
        xor_acc ^= val;
        sub = (sub - 1) & k;
    } while (sub != k);   /* loops until all submasks processed */

    return xor_acc;
}
