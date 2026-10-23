/*
 * A044938  Number of runs of even length in the base-7 representation of n.
 * https://oeis.org/A044938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Number of runs of even length in the base-7 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* not used, but safe */

    idx_t prev = 8;      /* sentinel, larger than any base‑7 digit (0–6) */
    idx_t run_len = 0;
    seq_t total = 0;     /* result to return */

    while (n > 0) {
        idx_t digit = n % 7;
        if (digit == prev) {
            run_len++;
        } else {
            if (run_len % 2 == 0 && run_len > 0)
                total++;
            run_len = 1;
            prev = digit;
        }
        n /= 7;
    }

    /* check the final run */
    if (run_len % 2 == 0 && run_len > 0)
        total++;

    return total;
}
