/*
 * A069226  a(n) = gcd(n, 2^n + 1).
 * https://oeis.org/A069226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, 2^n + 1)
 * Range:     u8 n <= 233, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 0.
       a(0) = gcd(0, 2^0 + 1) = gcd(0, 2) = 2. */
    if (n == 0) {
        return (seq_t)2;
    }

    /* Convert n to seq_t once for reuse. */
    seq_t n_seq = (seq_t)n;

    /* Compute 2^n mod n. */
    seq_t pow_mod = mod_pow((seq_t)2, n, n_seq);

    /* Compute (2^n + 1) mod n. */
    seq_t x = (pow_mod + (seq_t)1) % n_seq;

    /* If the remainder is 0, then 2^n + 1 is a multiple of n, so the gcd is n itself. */
    if (x == (seq_t)0) {
        return n_seq;
    }

    /* Otherwise, the gcd is gcd(n, x). */
    return gcd(n_seq, x);
}
