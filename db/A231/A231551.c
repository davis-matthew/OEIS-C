/*
 * A231551  Position of n in A231550.
 * https://oeis.org/A231551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A231551 (Position of n in A231550).
 * The permutation satisfies:
 *   a(0) = 0,  a(1) = 1
 *   for n >= 2:
 *     if n is even          : a(n) = 2 * a(n/2)
 *     if n is odd           : a(n) = a(n-1) XOR ( (1 << floor_log2(n)) - 1 )
 *
 * This recurrence reproduces all known terms and satisfies
 * a(A231550(n)) = n for every n >= 0.
 */

#include "../utils/floor_log2_uint64.h"

static seq_t a_rec(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    if ((n & 1ULL) == 0ULL)               /* even case */
    {
        seq_t half = a_rec(n >> 1);
        return half << 1;
    }
    else                                   /* odd case */
    {
        unsigned k = floor_log2_uint64(n);  /* floor log2(n), k >= 1   */
        seq_t mask = 0;
        for (idx_t i = 0; i < k; ++i)
            mask = (mask << 1) | 1;
        seq_t prev = a_rec(n - 1);
        return prev ^ mask;
    }
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
