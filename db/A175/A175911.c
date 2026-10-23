/*
 * A175911  Concatenate the run lengths of the runs of ones and zeros in the
 *          binary representation of n in the lowest possible base where it is
 *          possible to represent each run length as a single digit. Convert
 *          the result to base 10.
 * https://oeis.org/A175911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Interpret the run lengths of the binary runs of n * as
 *            digits in the smallest base ≥ (max run length + 1). * See OEIS
 *            A175911 for the definition
 * Range:     u8 n <= 68, u16 n <= 980, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Sequence is only defined for n >= 1.  Return 0 for safety otherwise. */
    if (n == 0)
        return 0;

    /* Work on a 64‑bit value, as idx_t is uint64_t. */
    uint64_t m = (uint64_t)n;

    /* Find the most significant bit position of m. */
    int msb = 63;
    while ((m >> msb) == 0U)
        msb--;

    /* Array to hold run lengths.  At most 64 runs for a 64‑bit integer. */
    seq_t runs[65];
    size_t run_count = 0;
    int    current_bit = 1;      /* the highest‑bit is always 1 */
    int    run_len    = 1;       /* length of the current run */
    int    max_run    = 1;       /* largest run length seen so far */

    /* Scan the remaining bits from msb‑1 down to 0. */
    for (int i = msb - 1; i >= 0; --i) {
        int bit = (int)((m >> i) & 1U);
        if (bit == current_bit) {
            ++run_len;
        } else {
            runs[run_count++] = (seq_t)run_len;
            if (run_len > max_run)
                max_run = run_len;
            current_bit = bit;
            run_len     = 1;
        }
    }

    /* Record the final run. */
    runs[run_count++] = (seq_t)run_len;
    if (run_len > max_run)
        max_run = run_len;

    /* Lowest possible base that can represent all digits. */
    seq_t base = (seq_t)(max_run + 1);

    /* Convert the run lengths (digits) to a base‑10 number. */
    seq_t result = 0;
    for (size_t i = 0; i < run_count; ++i) {
        result = result * base + runs[i];
    }

    return result;
}
