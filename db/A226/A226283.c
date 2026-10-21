/*
 * A226283  a(n) = [n/2]!*[(n+1)/2]!*C([n/2],2)*C([(n+1)/2],2).
 * https://oeis.org/A226283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/2⌋! * ⌊(n+1)/2⌋! * C(⌊n/2⌋,2) * C(⌊(n+1)/2⌋,2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = n / 2;
    idx_t k = (n + 1) / 2;

    seq_t fm = factorial(m);
    seq_t fk = factorial(k);
    seq_t cmb_m = binom(m, 2);
    seq_t cmb_k = binom(k, 2);

    seq_t res = fm * fk;
    res *= cmb_m;
    res *= cmb_k;
    return res;
}
