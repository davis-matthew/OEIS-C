/*
 * A065286  The bisection of odd terms (the <= 0 half of Z) of A065285.
 * https://oeis.org/A065286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A065286 – Bisection of odd terms (the ≤0 half of Z) of A065285
 *
 *  The sequence can be described in terms of binary blocks:
 *  For n ≥ 3 let                                 
 *      m   = ⌊log₂(n‑1)⌋ + 1          (so 2^(m‑1) < n ≤ 2^m )
 *      B   = 2^(m‑1)                   (base of the current block)
 *      H   = 2^(m‑2)                   (half‑size of the block)
 *      O   = n – B – 1                 (0‑based offset inside the block)
 *
 *  If O < H   →  a(n)/2 = B + O
 *  else        →  a(n)/2 = H
 *
 *  Finally  a(n) = 2 * (a(n)/2).  For n = 1, a(1)=0; for n = 2, a(2)=2.
 *
 *  This follows the observed pattern:
 *      1: 0
 *      2: 2
 *      3: 4
 *      4: 2
 *      5: 8
 *      6: 10
 *      7: 4
 *      8: 4
 *      9: 16
 *     10: 18
 *     11: 20
 *     12: 22
 *     13: 8
 *     14: 8
 *     15: 8
 *     16: 8
 *     17: 32
 *     18: 34
 *     19: 36
 *     20: 38
 *     21: 40
 *     22: 42
 *     23: 44
 *     24: 46
 *
 *  The implementation below is O(log n) for each call, easily
 *  below the 2 s time limit for n up to 10 000.
 */

#include <stdint.h>

static inline idx_t floor_log2_u64(idx_t v)
{
    /* Assumes v > 0; returns floor(log2(v)) */
    idx_t r = 0;
    while (v >>= 1U) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 2;

    /* n ≥ 3 */
    idx_t m = floor_log2_u64(n - 1) + 1;          /* ceil(log2(n)) */
    seq_t base = (seq_t)1 << (m - 1);            /* 2^(m‑1)   */
    seq_t half = (seq_t)1 << (m - 2);            /* 2^(m‑2)   */

    idx_t offset = n - (idx_t)base - 1;           /* 0‑based position inside the block */
    seq_t value;
    if (offset < (idx_t)half)                    /* first half of block */
        value = base + offset;
    else                                         /* second half of block */
        value = half;

    return value << 1;                            /* multiply by 2 */
}
