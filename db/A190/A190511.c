/*
 * A190511  n+[nr/u]+[ns/u]+[nt/u]; r=golden ratio, s=r^2, t=r^3, u=r^4.
 * https://oeis.org/A190511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/r³⌋ + ⌊n/r²⌋ + ⌊n/r⌋ + n, with r = (1+√5)/2 (the
 *            golden ratio)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived identities: 
   1/r   = r−1 = (√5−1)/2
   1/r²  = (3−√5)/2
   1/r³  = √5−2
   Hence using S = ⌊n·√5⌋,
   ⌊n/r⌋   = (S − n) / 2
   ⌊n/r²⌋ = (3·n − S − 1) / 2
   ⌊n/r³⌋ =  S − 2·n
*/

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t */
    seq_t n_seq = (seq_t)n;

    /* S = floor(n * sqrt(5))  = floor( sqrt(5 * n^2) ) */
    seq_t sq = n_seq * n_seq;                /* n² */
    sq = sq * (seq_t)5;                     /* 5·n² */
    seq_t S = floor_sqrt(sq);                /* floor(n·√5) */

    /* ⌊n/r⌋ = (S − n) / 2 */
    seq_t part1 = (S - n_seq) / (seq_t)2;

    /* ⌊n/r²⌋ = (3·n − S − 1) / 2 */
    seq_t part2_num = (n_seq + n_seq + n_seq) - S - (seq_t)1;
    seq_t part2 = part2_num / (seq_t)2;

    /* ⌊n/r³⌋ = S − 2·n */
    seq_t part3 = S - (n_seq << 1);

    /* a(n) = n + part1 + part2 + part3 */
    return n_seq + part1 + part2 + part3;
}
