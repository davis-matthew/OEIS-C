/*
 * A066809  a(n) = sum of modular offsets: mod[n+c,b]-(mod[n,b]+c) for
 *          c<=b<=n.
 * https://oeis.org/A066809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{b=1..n} b * ((n mod b) + 1) * = Σ_{b=1..n} b*(n mod b)
 *            + n(n+1)/2 * This implementation runs in O(n) time, suitable for
 *            n up to about 10 000
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1. For n = 0 we return 0. */
    if (n == 0) return 0;

    seq_t sum_mod = 0;
    for (idx_t b = 1; b <= n; ++b) {
        seq_t rem = (seq_t)n % (seq_t)b;
        sum_mod += (seq_t)b * rem;
    }

    seq_t half = ((seq_t)n * ((seq_t)n + 1)) / 2;
    return sum_mod + half;
}
