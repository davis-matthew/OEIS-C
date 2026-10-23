/*
 * A126236  Maximum length of a codeword in Huffman encoding of n symbols,
 *          where the k-th symbol has frequency k.
 * https://oeis.org/A126236
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(log_2(n)) + floor(log_2(2*n/3))
 * Computes the maximum Huffman codeword length for n symbols with
 * frequencies 1..n.
 */

#include <stdint.h>

static inline unsigned floor_log2_uint64(unsigned long long v)
{
    /* v > 0 is guaranteed for the use in this function */
    return 63 - __builtin_clzll(v);
}

static inline unsigned floor_log2_u128(unsigned __int128 v)
{
    /* v > 0 is guaranteed */
    unsigned long long high = (unsigned long long)(v >> 64);
    if (high)
        return 64 + floor_log2_uint64(high);
    else
        return floor_log2_uint64((unsigned long long)v);
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2 (offset 2). For safety, return 0 for n < 2. */
    if (n < 2)
        return (seq_t)0;

    unsigned long long log_n = floor_log2_uint64((unsigned long long)n);

    unsigned __int128 tmp = (unsigned __int128)n * 2ULL;
    tmp /= 3ULL;
    unsigned long long log_tmp = floor_log2_u128(tmp);

    return (seq_t)(log_n + log_tmp);
}
