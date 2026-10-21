/*
 * A062912  Non-palindromic number and its reversal are both multiples of 13.
 * https://oeis.org/A062912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth non‑palindromic number such that both the number
 *            * and its decimal reverse are multiples of 13
 * Range:     u8 none, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        seq_t cand = (seq_t)13 * (seq_t)m;
        seq_t rev  = reverse_seq(cand);
        if (cand != rev && rev % (seq_t)13 == 0) {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
