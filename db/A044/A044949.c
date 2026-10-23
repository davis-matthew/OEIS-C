/*
 * A044949  Number of runs of odd length in the base-9 representation of n.
 * https://oeis.org/A044949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Number of runs of odd length in the base‑9 representation of n
 * Range:     u8 n <= 9026, u16 n <= 9026, u32 n <= 9026, u64 n <= 9026,
 *            u128 n <= 9026, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle case n == 0 explicitly; offset starts at 1. */
    if (n == 0)
        return (seq_t)0;

    /* Base‑9 representation: at most 24 digits for 64‑bit n. */
    uint8_t digits[64];
    size_t pos = 0;
    idx_t tmp = n;
    while (tmp > 0)
    {
        digits[pos++] = (uint8_t)(tmp % 9);
        tmp /= 9;
    }

    /* Count runs of odd length. */
    size_t odd_runs = 0;
    size_t run_len  = 1;
    uint8_t prev_digit = digits[pos - 1];

    for (size_t i = pos - 1; i-- > 0; )
    {
        if (digits[i] == prev_digit)
        {
            run_len++;
        }
        else
        {
            if (run_len & 1)   /* odd length? */
                odd_runs++;
            prev_digit = digits[i];
            run_len = 1;
        }
    }

    /* Final run. */
    if (run_len & 1)
        odd_runs++;

    return (seq_t)odd_runs;
}
