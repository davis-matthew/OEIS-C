/*
 * A096112  Duplicate of A052330.
 * https://oeis.org/A096112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is built from two parts: 1. A base block of 8 integers: B =
 *            [1,2,3,6,4,8,12,24] each element is 2^e2 * 3^e3 with
 *            e2∈{0,1,2,3} and e3∈{0,1} (order: e3=0 group first, then e3=1
 *            group; within each group e2=0,1 then e2=2,3); 2. For n>8, write
 *            n-1 = 8⋅k + r (r∈[0,7]). k (the “block” index) determ...
 * Range:     i8 n <= 23, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t idx = n - 1;               /* zero‑based index */
    idx_t block = idx >> 3;          /* idx / 8  */
    idx_t pos   = idx & 7;           /* idx % 8  */

    /* exponents of 2 and 3 for the 8 base values, in the exact order above */
    static const uint8_t e2[8] = {0,1,0,1,2,3,2,3};
    static const uint8_t e3[8] = {0,0,1,1,0,0,1,1};

    /* precomputed small powers */
    static const seq_t pow2[4] = {(seq_t)1,(seq_t)2,(seq_t)4,(seq_t)8};
    static const seq_t pow3[2] = {(seq_t)1,(seq_t)3};

    seq_t base = pow2[e2[pos]] * pow3[e3[pos]];   /* 2^e2 * 3^e3 */

    /* list of primes > 3, enough for 11 or more bits (covers n up to >4000) */
    static const uint64_t primes_ex[] = {
        5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL,41ULL,
        43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,73ULL,79ULL,83ULL,
        89ULL,97ULL,101ULL,103ULL,107ULL,109ULL,113ULL,127ULL,131ULL,
        137ULL,139ULL,149ULL,151ULL,157ULL,163ULL,167ULL,173ULL,179ULL,
        181ULL,191ULL,193ULL,197ULL,199ULL,211ULL,223ULL,227ULL,229ULL,
        233ULL,239ULL
    };
    /* compute product of selected primes (bits of block) */
    seq_t mult = (seq_t)1;
    idx_t bit = 0;
    idx_t tmp = block;
    while (tmp) {
        if (tmp & 1ULL) {
            mult *= (seq_t)primes_ex[bit];
        }
        tmp >>= 1;
        bit++;
    }

    return base * mult;
}
