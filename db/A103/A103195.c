/*
 * A103195  Concatenations of pairs of primes that differ by four.
 * https://oeis.org/A103195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(A023200(n), A023200(n)+4) * A023200(n) is the nth
 *            cousin prime (primes that differ by 4). * The function returns
 *            the decimal concatenation of the pair
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digit_len, is_prime_uint64, pow10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/pow10_idx.h"
#include "../utils/digit_len.h"

/* Concatenate a and b in decimal notation. */
static seq_t concat(seq_t a, seq_t b)
{
    idx_t d   = digit_len(b);
    seq_t mul = pow10_idx(d);   /* 10^d as seq_t */
    return a * mul + b;
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for indices < 1 */

    uint64_t count = 0;
    uint64_t cand  = 3;     /* first odd number > 2 */

    for (;;) {
        if (is_prime_uint64(cand) && is_prime_uint64(cand + 4)) {
            count++;
            if (count == n) break;
        }
        cand += 2;          /* only odd candidates are needed */
    }

    seq_t p = (seq_t)cand;
    seq_t q = p + (seq_t)4;
    return concat(p, q);
}
