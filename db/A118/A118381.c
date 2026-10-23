/*
 * A118381  Largest 3-smooth number dividing n!.
 * https://oeis.org/A118381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(v₂(n!)) * 3^(v₃(n!))
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 75,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v_p(m) is the exponent of the prime p in the factorization of m. */

#include "../utils/seq_mul.h"

static seq_t pow_base(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        result = seq_mul(result, base);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t e2 = 0, e3 = 0;
    for (idx_t q = n; q > 1; q /= 2) e2 += q / 2;
    for (idx_t q = n; q >= 3; q /= 3) e3 += q / 3;
    seq_t res2 = pow_base((seq_t)2, e2);
    seq_t res3 = pow_base((seq_t)3, e3);
    return seq_mul(res2, res3);
}
