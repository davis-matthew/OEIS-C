/*
 * A193889  Base at which the first composite occurs in the sequence
 *          p=prime(n), p1=base(p,9), p2=base(p1,8), p3=base(p2,7),..., where
 *          base(N,b) means N written in base b and read in base 10; a(n)=0 if
 *          p1,...,p8 are all prime.
 * https://oeis.org/A193889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the base of the first composite in the sequence obtained
 *            by repeatedly converting prime(n) from base 9 down to base 2.
 *            Returns 0 if all intermediate results are prime
 * Range:     u8 n <= 30, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   base_convert, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/base_convert.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)get_prime(n);   /* prime(n) */
    for (int b = 9; b >= 2; --b) {
        seq_t nxt = base_convert(cur, b);
        if (!is_prime_big(nxt))
            return (seq_t)b;
        cur = nxt;
    }
    return 0;
}
