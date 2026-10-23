/*
 * A100893  a(n) = smallest n-digit prime formed by appending a digit to
 *          a(n-1); a(1) = 2.
 * https://oeis.org/A100893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = smallest n-digit prime formed by appending a digit to
 *            a(n-1), a(1)=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_seq.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) = 2 */
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 */

    seq_t val = (seq_t)2;
    if (n == 1)
        return val;

    for (idx_t i = 2; i <= n; ++i) {
        bool found = false;
        seq_t newval = (seq_t)0;

        for (int d = 0; d <= 9; ++d) {
            seq_t cand = val * (seq_t)10 + (seq_t)d;
            if (is_prime_seq(cand)) {
                newval = cand;
                found = true;
                break;
            }
        }

        if (!found)          /* no n‑digit prime can be formed */
            return (seq_t)0; /* sequence terminates */

        val = newval;
    }
    return val;
}
