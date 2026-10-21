/*
 * A068634  a(n) = lcm(n, R(n)), where R(n) (A004086) = digit reversal of n.
 * https://oeis.org/A068634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look, changed
 * Formula:   a(n) = lcm(n, R(n))
 * Range:     u8 n <= 12, u16 n <= 106, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* R(n) is the decimal digit reversal of n. */

#include "../utils/gcd.h"
#include <stdint.h>

/* Reverse the decimal digits of a 64‑bit integer. */
static uint64_t reverse_uint64(uint64_t x) {
    uint64_t r = 0;
    while (x > 0) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Convert the index to seq_t. */
    seq_t n_seq = (seq_t)n;

    /* Compute R(n). */
    seq_t rev_seq = (seq_t)reverse_uint64(n);

    /* Compute gcd and lcm. */
    seq_t g   = gcd(n_seq, rev_seq);
    seq_t q   = n_seq / g;

    /* Use 128‑bit intermediary so that the product does not wrap
       in the 64‑bit build (the result will be truncated to seq_t
       which is harmless for values that fit in seq_t). */
    unsigned __int128 prod = (unsigned __int128)q * (unsigned __int128)rev_seq;

    return (seq_t)prod;
}
