/*
 * A072892  The 4-cycle of the n => sigma(n)-n process. sigma(n) is the sum of
 *          divisors of n. (A000203).
 * https://oeis.org/A072892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n+4) = a(n); a(1) = 1264460, a(2) = 1547860, a(3) = 1727636,
 *            a(4) = 1305184
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Returns the nth term of OEIS sequence A072892.
   The sequence repeats every 4 terms. */
seq_t a(idx_t n)
{
    /* constant cycle of length 4 */
    static const seq_t cycle[4] = {
        1264460,    /* a(1) */
        1547860,    /* a(2) */
        1727636,    /* a(3) */
        1305184     /* a(4) */
    };

    /* n is 1-based; index into cycle is (n-1) mod 4 */
    idx_t r = (n - 1) % 4;
    return cycle[r];
}
