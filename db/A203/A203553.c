/*
 * A203553  Lodumo_2 of A118175, which is n 1's followed by n 0's.
 * https://oeis.org/A203553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lodumo_2(A118175(n)) (permutation of non‑negative
 *            integers)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Computes the n‑th term of A203553 */
seq_t a(idx_t n)
{
    /* Find the smallest k such that k*(k+1) > n.
       k represents the current block size (odd block of size k followed by an even block of size k). */
    idx_t k = (idx_t)sqrt((long double)n);
    if (k == 0) k = 1;
    while ((seq_t)k * ((seq_t)k + 1) <= (seq_t)n) ++k;

    /* The block for this k starts at index k*(k-1) */
    seq_t block_start = (seq_t)k * ((seq_t)k - 1);
    seq_t offset      = (seq_t)n - block_start;            /* 0 <= offset < 2*k */

    /* Number of odds (or evens) before this block */
    seq_t base = (seq_t)(k - 1) * k / 2;

    if (offset < (seq_t)k) {          /* odd block */
        seq_t idx = base + offset;    /* index in the global odd list (0‑based) */
        return 1 + (seq_t)2 * idx;    /* odd numbers are 1,3,5,… */
    } else {                          /* even block */
        seq_t idx = base + (offset - (seq_t)k); /* index in the global even list */
        return (seq_t)2 * idx;        /* even numbers are 0,2,4,… */
    }
}
