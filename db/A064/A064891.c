/*
 * A064891  "Binary potency" of n: length of shortest blocks of 0's inserted
 *          between the bits of n required to "dilute" it into a nonprime.
 * https://oeis.org/A064891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binary potency of n: the length of the shortest block of 0's that must
   be inserted between the bits of n to make a non‑prime.  For non‑prime
   n the result is 0. */
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"

static seq_t dilate_bits(seq_t n, unsigned k)
{
    seq_t res = 0;
    unsigned len = bitlen_seq_t(n);
    for (int i = (int)len - 1; i >= 0; --i) {
        seq_t bit = (n >> i) & 1;
        res = (res << 1) | bit;
        if (i > 0) res <<= k;          /* insert k zeros between bits   */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    if (!is_prime_big(x)) return (seq_t)0;  /* n is composite → potency 0 */

    for (unsigned k = 1; ; ++k) {
        seq_t v = dilate_bits(x, k);
        if (!is_prime_big(v)) return (seq_t)k; /* first composite dilation */
    }
}
