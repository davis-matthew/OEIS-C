/*
 * A075171  Nonnegative integers mapped to Dyck path encodings of the rooted
 *          plane trees obtained by recursing on the run lengths of the binary
 *          expansion of n.
 * https://oeis.org/A075171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by reading the binary representation of n. For
 *            each maximal run of identical bits of length L if L is odd: the
 *            run contributes the Dyck word 1 ( (10) repeated (L-1) times ) 0
 *            if L is even: the run contributes the word (L/2+1) times '1'
 *            followed by (L/2+1) times '0'. All produced...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   append_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_run.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* find most significant set bit (msb position) */
    int msb = 0;
    for (int i = 63; i >= 0; --i)
        if ((n >> i) & 1) { msb = i; break; }

    seq_t result = 0;
    int prev_bit = (n >> msb) & 1;
    idx_t run_len = 1;

    for (int i = msb - 1; i >= 0; --i) {
        int bit = (n >> i) & 1;
        if (bit == prev_bit) {
            ++run_len;
        } else {
            append_run(&result, run_len);
            run_len = 1;
            prev_bit = bit;
        }
    }
    append_run(&result, run_len);   /* last run */
    return result;
}
