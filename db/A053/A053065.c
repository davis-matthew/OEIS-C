/*
 * A053065  Alternately append n-th prime to beginning or end of previous
 *          term.
 * https://oeis.org/A053065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = a(1)=2. For n>1, * a(n) = concat(a(n-1), p_n) if n is
 *            odd, * concat(p_n, a(n-1)) if n is even, * where p_n is the n‑th
 *            prime and concat(x,y)=x·10^{digits(y)}+y
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   digit_len, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Compute A053065 */
seq_t a(idx_t n)
{
    /* The first term is simply the first prime. */
    seq_t cur = 2;

    /* Build the sequence up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t p = (seq_t)get_prime(i);          /* the i‑th prime      */
        if (i & 1U) {                           /* odd index – append */
            cur = cur * pow10((idx_t)digit_len(p)) + p;
        } else {                                /* even index – prepend */
            cur = p * pow10((idx_t)digit_len(cur)) + cur;
        }
    }
    return cur;
}
