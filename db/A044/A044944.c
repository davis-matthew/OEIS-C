/*
 * A044944  Runs of odd length in the base 4 representation of n.
 * https://oeis.org/A044944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs of odd length in the base‑4 representation
 *            of n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Extract base‑4 digits (least‑significant first) */
    unsigned digits[65];
    int dcount = 0;
    idx_t tmp = n;
    do {
        digits[dcount++] = (unsigned)(tmp & 3ULL);
        tmp >>= 2;
    } while (tmp);

    int count = 0;
    int run_len = 1;
    for (int i = dcount - 1; i > 0; --i) {
        if (digits[i] == digits[i - 1]) {
            ++run_len;
        } else {
            if (run_len & 1)
                ++count;
            run_len = 1;
        }
    }
    if (run_len & 1)
        ++count;
    return (seq_t)count;
}
