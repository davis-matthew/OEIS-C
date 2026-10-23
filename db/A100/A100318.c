/*
 * A100318  Numbers n such that at least one of n-1 and n+1 is composite.
 * https://oeis.org/A100318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer n such that at least one of n-1 or n+1 is
 *            composite
 * Range:     u8 n <= 236, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = (seq_t)3; /* a(1) = 3 */

    for (;; ++x) {
        bool ok;

        if (x == 2) {
            ok = false;                    /* 2 is excluded */
        } else if ((x & 1) != 0) {
            /* All odd integers > 1 are included */
            ok = true;
        } else {
            seq_t lo = x - 1;
            seq_t hi = x + 1;
            /* Include if at least one neighbour is composite */
            ok = !(is_prime_seq(lo) && is_prime_seq(hi));
        }

        if (ok) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
