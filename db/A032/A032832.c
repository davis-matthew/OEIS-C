/*
 * A032832  Numbers whose set of base-8 digits is {3,4}.
 * https://oeis.org/A032832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose set of base‑8 digits is {3,4}.  Sequence A032832.
   The nth term is obtained by interpreting (n‑1) in binary, mapping
   0 → 3 and 1 → 4, and treating the resulting digits as a base‑8
   number. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the number of digits L such that 2^L <= n+1 < 2^(L+1). */
    idx_t limit = n + 1;
    int L = 0;
    for (int i = 63; i >= 0; --i) {
        if ((limit >> i) & 1U) {
            L = i;
            break;
        }
    }

    /* Position of n within the block of length L numbers. */
    idx_t offset = n - (((idx_t)1 << L) - 1);

    seq_t val = 0;
    for (int i = L - 1; i >= 0; --i) {
        int bit = (offset >> i) & 1U;
        val = val * 8 + (bit ? 4 : 3);
    }
    return val;
}
