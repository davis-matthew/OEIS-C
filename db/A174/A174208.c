/*
 * A174208  Write natural numbers in base 2 as a stream of digits. Moving left
 *          to right, delete odd occurrences of digit 0 and even occurrences
 *          of digit 1.
 * https://oeis.org/A174208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n-th bit (0‑based) in the stream produced by writing
 *            the binary representations of natural numbers in order, deleting
 *            odd occurrences of 0 and even occurrences of 1
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t zeros_cnt = 0;      /* count of 0s seen so far (overall)          */
    idx_t ones_cnt  = 0;      /* count of 1s seen so far (overall, all kept)*/
    idx_t kept_idx  = 0;      /* number of bits kept so far                 */

    for (idx_t x = 0; ; ++x)
    {
        /* produce the bits of x from most significant to least significant */
        if (x == 0)
        {
            /* special case: single bit 0 */
            zeros_cnt++;
            if ((zeros_cnt & 1) == 0) {    /* keep even‑indexed 0s */
                if (kept_idx == n) return (seq_t)0;
                kept_idx++;
            }
            continue;
        }

        /* find most significant bit index */
        unsigned long long v = (unsigned long long)x;
        int bitlen = 0;
        while (v) { bitlen++; v >>= 1; }

        for (int i = bitlen - 1; i >= 0; --i)
        {
            idx_t bit = (x >> i) & 1;
            if (bit == 0)
            {
                zeros_cnt++;
                if ((zeros_cnt & 1) == 0) {    /* keep even‑indexed 0s */
                    if (kept_idx == n) return (seq_t)0;
                    kept_idx++;
                }
            }
            else /* bit == 1 */
            {
                ones_cnt++;
                if ((ones_cnt & 1) == 1) {    /* keep odd‑indexed 1s */
                    if (kept_idx == n) return (seq_t)1;
                    kept_idx++;
                }
            }

            if (kept_idx > n) return (seq_t)0; /* should never happen */
        }
    }
}
