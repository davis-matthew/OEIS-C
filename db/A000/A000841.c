/*
 * A000841  Number of n-input 2-output switching networks under action of
 *          symmetric group S(n) on the inputs and complementing group C(2,2)
 *          on the outputs.
 * https://oeis.org/A000841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma. The all-zero input is fixed by every
 *            permutation, so a nonzero output complement fixes no function.
 *            Hence a(n) = (1/(4*n!)) * Sum_{sigma in S(n)} 4^c(sigma), where
 *            c(sigma) is the number of cycles of sigma acting on {0,1}^n.
 *            Grouping by cycle type lambda gives a(n) = (1/4) * Sum...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   a841_gcd, a841_pow2, a841_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a841_gcd.h"
#include "../utils/a841_pow2.h"
#include "../utils/a841_rec.h"

seq_t a(idx_t n) {
    int parts[64];
    uint64_t fact = 1;
    int nn;
    if (n < 1 || n > 20) return 0;
    nn = (int)n;
    for (int i = 2; i <= nn; i++) fact *= (uint64_t)i;
    if (nn <= 5) {
        unsigned __int128 s = 0;
        seq_t dummy = 0;
        a841_rec(nn, nn, parts, 0, fact, 0, &s, &dummy);
        s /= (unsigned __int128)fact * 4;
        return (seq_t)(uint64_t)s;
    } else {
        unsigned __int128 dummy = 0;
        seq_t s = 0;
        a841_rec(nn, nn, parts, 0, fact, 1, &dummy, &s);
        return s / ((seq_t)fact * 4);
    }
}
