/*
 * A035032  For n >= 6, max( prevprime(n), 2*prevprime(floor(n/2))).
 * https://oeis.org/A035032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(prev_prime(n), 2*prev_prime(n/2)) for n ≥ 6, with
 *            a(1)=1, a(2)=2, a(3)=2, a(4)=3, a(5)=3
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    static const seq_t init[5] = { (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)3, (seq_t)3 };
    if (n <= 5) {
        return init[n - 1];
    }
    idx_t pp      = prev_prime(n);
    idx_t half_pp = prev_prime(n / 2);
    seq_t v1 = (seq_t)pp;
    seq_t v2 = ((seq_t)half_pp) * ((seq_t)2);
    return (v1 > v2) ? v1 : v2;
}
