/*
 * A100736  Inverse modulo 2 binomial transform of 3^n.
 * https://oeis.org/A100736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^A010060(n-k)*(binomial(n, k) mod 2)*3^k
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A010060(n) is the Thue-Morse sequence, (−1)^A010060(m) = (+1 or –1) depending on
   the parity of popcount(m).  binomial(n,k) mod 2 is 1 iff k is a submask of n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = n;;) {
        idx_t rem = n - k;
        int parity = __builtin_popcountll(rem) & 1; /* 0 even, 1 odd */

        seq_t term = pow_seq((seq_t)3, k);          /* 3^k */

        if (parity == 0)
            sum += term;        /* even parity -> + */
        else
            sum -= term;        /* odd parity -> - */

        if (k == 0) break;     /* last submask processed */
        k = (k - 1) & n;        /* next submask of n */
    }
    return sum;
}
