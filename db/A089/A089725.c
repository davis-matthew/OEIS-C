/*
 * A089725  Let f(n) = (2n+1), g(n) = (n-1)/2. Then a(n) = f(f(f(...(n)))),
 *          where one gets prime at every step. If f(n) is not a prime then
 *          a(n) = g(g(g...(n)))) where one gets a prime at every step. If
 *          f(n) and g(n) both are composite then a(n) = 0.
 * https://oeis.org/A089725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A089725: compute by repeated application of f(n)=2n+1 while prime,
   otherwise apply g(n)=(n-1)/2.  If a prime is never reached, return 0. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;

    /* first try the f chain */
    seq_t curr = val * (seq_t)2 + (seq_t)1;
    if (is_prime_big(curr)) {
        seq_t last = curr;
        while (1) {
            curr = curr * (seq_t)2 + (seq_t)1;
            if (is_prime_big(curr)) {
                last = curr;
            } else {
                return last;
            }
        }
    }

    /* f chain failed – try the g chain.  g is defined only for odd n. */
    if ((val & (seq_t)1) == (seq_t)0) {
        return (seq_t)0;
    }
    curr = (val - (seq_t)1) / (seq_t)2;
    if (!is_prime_big(curr)) {
        return (seq_t)0;
    }

    seq_t last = curr;
    while (1) {
        /* next g value exists only if current is odd */
        if ((curr & (seq_t)1) == (seq_t)0) {
            break;
        }
        curr = (curr - (seq_t)1) / (seq_t)2;
        if (is_prime_big(curr)) {
            last = curr;
        } else {
            break;
        }
    }
    return last;
}
