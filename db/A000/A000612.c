/*
 * A000612  Number of P-equivalence classes of switching functions of n or
 *          fewer variables, divided by 2.
 * https://oeis.org/A000612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   Burnside over cycle types lambda of S_n: a(n) = Sum_{lambda |-
 *            n} 2^(c(lambda)-1) / z(lambda), where c(lambda) = number of
 *            cycles of a permutation of type lambda acting on all subsets of
 *            an n-set, c = (1/L) * Sum_{k=0..L-1} 2^(Sum_i m_i*gcd(i,k)), L =
 *            lcm of the parts, z(lambda) = Prod_i i^m_i *...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   h612_gcd, h612_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/h612_gcd.h"
#include "../utils/h612_rec.h"

seq_t a(idx_t n) {
    /* detect fixed width of seq_t */
    seq_t x = 1;
    int w = 0;
    while (x != 0 && w < 300) { x <<= 1; w++; }
    if (w < 300) {
        if (n >= 8 || ((uint64_t)1 << n) > (uint64_t)w) return 0; /* does not fit */
    }
    int nn = (int)n;
    int *m = (int *)calloc((size_t)nn + 2, sizeof(int));
    seq_t nf = 1;
    int i;
    for (i = 2; i <= nn; i++) nf *= (seq_t)i;
    seq_t total = 0;
    h612_rec(nn, nn, m, nn, nf, &total);
    free(m);
    return total / nf;
}
