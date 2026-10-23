/*
 * A044933  Number of runs of even length in the base-2 representation of n.
 * https://oeis.org/A044933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9945, u16 n <= 9945, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = A005811(n) - A044942(n) */
/* Number of runs of even length in the base‑2 representation of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 (not expected for the sequence offset),
       but return 0 just in case. */
    if (n == 0) return 0;

    /* Determine the number of bits in n. */
    idx_t temp = n;
    unsigned bits = 0;
    while (temp) {
        bits++;
        temp >>= 1;
    }

    /* Scan the bits from most significant to least significant. */
    unsigned count_even_runs = 0;
    unsigned run_len = 1;
    int prev_bit = (int)((n >> (bits - 1)) & 1u);

    for (int i = (int)bits - 2; i >= 0; --i) {
        int bit = (int)((n >> i) & 1u);
        if (bit == prev_bit) {
            run_len++;
        } else {
            if ((run_len & 1u) == 0)
                count_even_runs++;
            prev_bit = bit;
            run_len = 1;
        }
    }
    /* account for the last run */
    if ((run_len & 1u) == 0)
        count_even_runs++;

    return (seq_t)count_even_runs;
}
