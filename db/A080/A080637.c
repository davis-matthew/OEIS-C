/*
 * A080637  a(n) is the smallest positive integer which is consistent with the
 *          sequence being monotonically increasing and satisfying a(1)=2,
 *          a(a(n)) = 2n+1 for n > 1.
 * https://oeis.org/A080637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1) = 2 * For n > 1: * let i = ⌊log2(n/2)⌋ (i ≥ 0) * let p =
 *            2^i * if n < 3p a(n) = n + p * else a(n) = 2*(n – p) + 1 * This
 *            implements the piece‑wise rule derived in the OEIS entry, *
 *            which yields the unique monotonic sequence with a(a(n)) = 2n+1
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 2;

    /* Find i such that 2^i ≤ n/2 < 2^(i+1)  */
    idx_t half = n >> 1;          /* floor(n/2)            */
    idx_t i    = 0;
    idx_t tmp  = half;
    while (tmp >>= 1) ++i;        /* i = floor(log2(half)) */

    seq_t p2 = 1;                 /* 2^i  */
    p2 <<= i;

    seq_t n_seq = (seq_t)n;
    seq_t three = p2 << 1;       /* 2 * p2  */
    three += p2;                 /* 3 * p2  */

    if (n_seq < three)
        return n_seq + p2;       /* a(n) = n + p            */
    else
        return (n_seq - p2) * 2 + 1; /* a(n) = 2(n - p) + 1  */
}
