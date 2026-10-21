/*
 * A065268  The bisection of odd terms (the <= 0 half of Z) of A065267.
 * https://oeis.org/A065268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A065268: The bisection of odd terms (the <= 0 half of Z) of A065267.
   Formula:  a(1)=a(2)=1.
             For n ≥ 3 let m = floor(log₂(n-1))  (thus 2^m < n ≤ 2^{m+1}).
             The interval (2^m, 2^{m+1}] is split into two halves.
             If n lies in the first half (i.e. n ≤ 2^m + 2^{m-1}),
                 a(n) = 3·2^m – 1   (an odd number).
             Otherwise (second half) a(n) = 2^m  (a power of two).

   This rule produces 1,1,5,2,11,11,4,4,23,23,23,23,8,8,8,8,47,47,47,47,47,47,47,47,
   and continues for all n ≥ 1.                                          */

#include <stdint.h>

/* Compute floor(log2(x)) for x > 0.  */
static idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n <= 2)
        return (seq_t)1;

    /* For n ≥ 3.  Compute m = floor(log₂(n‑1)).  */
    idx_t m = floor_log2_idx(n - 1);          /*  m ≥ 1  */
    idx_t block_start = ((idx_t)1 << m);      /* start index of current block  */
    idx_t offset = n - block_start - 1;      /* 0‑based offset within block  */
    idx_t half_len = ((idx_t)1 << (m - 1));   /* size of the first half of the block */

    if (offset < half_len) {                 /* first half – odd value */
        return (seq_t)3 * ((seq_t)1 << m) - (seq_t)1;
    } else {                                 /* second half – power of two */
        return (seq_t)1 << m;
    }
}
