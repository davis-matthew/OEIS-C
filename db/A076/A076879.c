/*
 * A076879  In binary representation: number of substrings of n^2 having the
 *          same number of 0's and 1's as n.
 * https://oeis.org/A076879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of substrings of binary(n^2) having the same
 *            number of 0’s and 1’s as the binary representation of n
 * Range:     u8 n <= 15, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   bitlen_seq_t_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t_custom.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for the sequence */
    unsigned len_n   = bitlen_seq_t_custom((seq_t)n);          /* length of binary(n)  */
    unsigned ones_n  = __builtin_popcountll((unsigned long long)n); /* popcount of n   */

    seq_t sq = (seq_t)n * (seq_t)n;                           /* n^2 as seq_t      */
    unsigned len_sq = bitlen_seq_t_custom(sq);                 /* length of binary(n^2) */

    if (len_n > len_sq) return 0;  /* impossible, but safe */

    unsigned long long ones_window = 0;

    /* initial window */
    for (unsigned i = 0; i < len_n; i++) {
        if ((sq >> i) & 1) ones_window++;
    }

    seq_t count = 0;
    if (ones_window == ones_n) count++;

    /* slide the window over binary(n^2) */
    for (unsigned i = 1; i + len_n <= len_sq; i++) {
        if ((sq >> (i - 1)) & 1) ones_window--;
        if ((sq >> (i + len_n - 1)) & 1) ones_window++;
        if (ones_window == ones_n) count++;
    }

    return count;
}
