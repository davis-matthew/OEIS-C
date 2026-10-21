/*
 * A212596  Number of cards required to build a Menger sponge of level n in
 *          origami.
 * https://oeis.org/A212596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*(8^n + 2*20^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for idx_t if needed */

/* Helper: fast exponentiation for arbitrary‑precision seq_t */
static seq_t pow_seq_local(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow8   = pow_seq_local((seq_t)8,  n);
    seq_t pow20  = pow_seq_local((seq_t)20, n);
    seq_t sum    = pow8 + ((seq_t)2 * pow20);
    return sum * ((seq_t)4);
}
