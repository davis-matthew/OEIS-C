/*
 * A175609  The difference between maximal run length and minimal run length
 *          in binary representation of n-th prime.
 * https://oeis.org/A175609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   bitlen_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = difference between maximal and minimal run lengths
   in the binary representation of the n-th prime.  */

#include "../utils/get_prime.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime. */
    uint32_t prime = get_prime(n);
    seq_t val = (seq_t)prime;

    /* Number of bits in the binary representation. */
    idx_t len = bitlen_seq_t(val);

    /* Run tracking. */
    seq_t max_run = 1;
    seq_t min_run = (seq_t)len;   /* initialized to upper bound */
    seq_t run    = 1;

    /* First bit (most significant). */
    int prev = (int)((val >> (len - 1)) & 1);

    /* Iterate over the remaining bits from most to least significant. */
    for (idx_t i = len - 1; i > 0; --i) {
        int bit = (int)((val >> (i - 1)) & 1);
        if (bit == prev) {
            run++;
        } else {
            if (run > max_run) max_run = run;
            if (run < min_run) min_run = run;
            run    = 1;
            prev   = bit;
        }
    }

    /* Update for the last run. */
    if (run > max_run) max_run = run;
    if (run < min_run) min_run = run;

    return max_run - min_run;
}
