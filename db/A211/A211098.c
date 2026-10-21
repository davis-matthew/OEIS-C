/*
 * A211098  Length of largest (i.e., leftmost) Lyndon word in Lyndon
 *          factorization of binary vectors of lengths 1, 2, 3, ...
 * https://oeis.org/A211098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the leftmost Lyndon word in the Chen–Fox–Lyndon
 *            factorization of the binary vector whose lexicographic index is
 *            n (starting with 0,1,00,01,10,11,000,001,010,...)
 * Range:     u8 n <= 121, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safe fallback */

    /* Find the length L of the binary vector at index n. */
    idx_t total = 0;
    idx_t L = 0;
    while (1) {
        ++L;
        total += ((idx_t)1 << L);          /* add 2^L: number of words of length L */
        if (total >= n)
            break;
    }

    idx_t prev_total = total - ((idx_t)1 << L);   /* total length up to L-1 */
    idx_t offset = n - prev_total - 1;           /* 0‑based offset within the block of length L */

    /* Build the binary word in an array of bits (0/1). */
    unsigned char bits[64];
    for (idx_t i = 0; i < L; ++i)
        bits[i] = (offset >> (L - 1 - i)) & 1;

    /* Duval's algorithm – only the first factor is needed. */
    idx_t j = 1, k = 0;               /* i is implicitly 0 */
    while (j < L && bits[k] <= bits[j]) {
        if (bits[k] < bits[j])
            k = 0;
        else
            ++k;
        ++j;
    }
    idx_t first_len = j - k;          /* length of the leftmost Lyndon factor */

    return (seq_t)first_len;
}
