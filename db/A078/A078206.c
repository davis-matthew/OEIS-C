/*
 * A078206  Smallest multiple of the concatenation of n down to 1 that begins
 *          with the concatenation of 1 up to n.
 * https://oeis.org/A078206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A078206: the smallest multiple of concat(n..1) that starts with concat(1..n)
   by searching over increasing lengths of suffix digits. */

#include <stdbool.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* build concatenation C = 1..n */
    seq_t C = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digits = pow10(digit_count((uint64_t)i));
        C = C * digits + i;
    }

    /* build concatenation P = n..1 */
    seq_t P = 0;
    for (idx_t i = n; i > 0; --i) {
        seq_t digits = pow10(digit_count((uint64_t)i));
        P = P * digits + i;
    }

    /* iterate over possible suffix lengths */
    seq_t pow10m = 1;  /* 10^m */
    for ( ;; ) {
        seq_t lower = C * pow10m;
        seq_t upper = (C + 1) * pow10m;   /* exclusive upper bound */

        seq_t k = (lower + P - 1) / P;     /* ceil(lower / P) */
        seq_t kP = k * P;

        if (kP < upper) {
            return kP;                    /* found smallest multiple */
        }

        pow10m *= 10;                     /* increase suffix length */
    }
}
