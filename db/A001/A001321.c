/*
 * A001321  Number of equivalence classes of 3-valued Post functions of n
 *          variables under action of symmetric group S_n.
 * https://oeis.org/A001321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n!) * Sum_{sigma in S_n} 3^c(sigma), where c(sigma) is
 *            the number of cycles of sigma acting on {0,1,2}^n, i.e. for a
 *            cycle type with parts l_1..l_m and L = lcm(l_i): c = (1/L) *
 *            Sum_{k=0..L-1} 3^(Sum_i gcd(k,l_i)); grouped by cycle type, a(n)
 *            = Sum_{lambda |- n} 3^c(lambda) / z_lambda
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   a1321_gcd, a1321_gen, a1321_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1321_gcd.h"
#include "../utils/a1321_gen.h"
#include "../utils/a1321_term.h"

seq_t a(idx_t n) {
    int nn = (int)n;
    uint64_t nf = 1;
    int i;
    for (i = 2; i <= nn; i++) nf *= (uint64_t)i;
    int *parts = (int *)malloc(sizeof(int) * (size_t)(nn + 1));
    seq_t Q = 0;
    unsigned __int128 R = 0;
    a1321_gen(nn, nn, parts, 0, &Q, &R, nf);
    free(parts);
    return Q + (seq_t)(uint64_t)(R / nf);
}
