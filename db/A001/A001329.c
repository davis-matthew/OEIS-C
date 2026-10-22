/*
 * A001329  Number of nonisomorphic groupoids with n elements.
 * https://oeis.org/A001329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n}
 *            fixA[s_1,s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...), fixA[s_1,s_2,...]
 *            = Product_{i,j>=1} (Sum_{d|lcm(i,j)} d*s_d)^(gcd(i,j)*s_i*s_j).
 *            Computed exactly as a(n) = (1/n!) * Sum_s (n!/z_s) * fixA[s],
 *            where n!/z_s is an integer
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a1329_fact, a1329_gcd, a1329_pow, a1329_rec, a1329_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a1329_fact.h"
#include "../utils/a1329_gcd.h"
#include "../utils/a1329_pow.h"
#include "../utils/a1329_rec.h"
#include "../utils/a1329_term.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t *s = (idx_t *)calloc(n, sizeof(idx_t));
    seq_t nfact = a1329_fact(n);
    seq_t sum = 0;
    a1329_rec(n, n, s, n, nfact, &sum);
    free(s);
    return sum / nfact;
}
