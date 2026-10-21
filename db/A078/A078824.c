/*
 * A078824  Number of distinct binary numbers contained as substrings in
 *          binary, circular formed representation of n and not longer than n.
 * https://oeis.org/A078824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   count the distinct binary numbers that appear as contiguous
 *            substrings (of any length 1..L) in the circular representation
 *            of n (where L is the usual binary length of n)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* compute binary length L (at least 1 for n==0) */
    idx_t L = 1;
    if (n != 0) {
        uint64_t tmp = n;
        L = 0;
        while (tmp) {
            tmp >>= 1;
            L++;
        }
    }

    /* upper bound for the number of possible substring values is 2^L */
    size_t cap = (size_t)1 << L;          /* cap == 2^L, L <= 14 for n <= 10000 */

    /* visited array (static to avoid reallocation) – size 32768 > 2^14 */
    static uint8_t visited[1 << 15];
    /* clear only the used part */
    memset(visited, 0, cap);

    /* extract bits of n into a small array */
    unsigned bits[64];
    for (idx_t i = 0; i < L; ++i)
        bits[i] = (n >> (L - 1 - i)) & 1;

    /* enumerate all circular substrings */
    for (idx_t start = 0; start < L; ++start) {
        seq_t value = 0;
        for (idx_t len = 1; len <= L; ++len) {
            unsigned bit = bits[(start + len - 1) % L];
            value = (value << 1) | bit;
            /* value is at most 2^L-1 and fits in idx_t, so cast safely */
            idx_t val = (idx_t)value;
            visited[val] = 1;
        }
    }

    /* count distinct values found */
    seq_t count = 0;
    for (size_t i = 0; i < cap; ++i)
        if (visited[i])
            count++;

    return count;
}
