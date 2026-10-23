/*
 * A162528  Numbers k whose largest divisor <= sqrt(k) equals 8.
 * https://oeis.org/A162528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*prime(n-2) for n > 8; the first eight terms are
 *            hard‑coded. (Because the OEIS definition gives a special finite
 *            set for the first eight indices, all later terms are simply 8
 *            times the (n‑2)ᵗʰ prime.)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* first eight special cases */
    static const seq_t first[8] = {64, 72, 80, 88, 96, 104, 112, 128};

    if (n <= 8)
        return first[n - 1];

    /* for n > 8 : 8 times the (n‑2)th prime */
    seq_t p = (seq_t)get_prime(n - 2);
    return p * (seq_t)8;
}
