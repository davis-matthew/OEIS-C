/*
 * A001115  Maximal number of pairwise relatively prime polynomials of degree
 *          n over GF(2).
 * https://oeis.org/A001115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(n) + Sum_{i=1..floor(n/2)} P(i), where P(n) =
 *            A001037(n) = (1/n)*Sum_{d|n} mu(n/d)*2^d, a(0) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   a1115_P, a1115_P_big, a1115_P_fixed, a1115_big_term,
 *            a1115_fixed_term, a1115_is_big, a1115_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1115_P.h"
#include "../utils/a1115_P_big.h"
#include "../utils/a1115_P_fixed.h"
#include "../utils/a1115_big_term.h"
#include "../utils/a1115_fixed_term.h"
#include "../utils/a1115_is_big.h"
#include "../utils/a1115_mu.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    int big = a1115_is_big();
    seq_t sum = a1115_P(n, big);
    idx_t i;
    for (i = 1; i <= n / 2; i++) {
        sum += a1115_P(i, big);
    }
    return sum;
}
