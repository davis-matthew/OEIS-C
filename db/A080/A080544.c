/*
 * A080544  In binary representation: keep the first digit and rotate right
 *          the others twice.
 * https://oeis.org/A080544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A080544 : “Keep the first binary digit and rotate the remaining bits right by two positions”.
    Algorithm: let L be the length of n in binary (number of bits).
    Keep the most significant bit (value 1<<(L-1)).
    Let R = n - (1<<(L-1))  (the remaining L-1 bits).
    Rotate R right by 2 places modulo (L-1):
        if L-1 == 0   → R' = 0
        else if L-1 == 1 → R' = R
        else:
            k = 2 % (L-1);
            R' = ((R >> k) | (R << ((L-1)-k))) & ((1ULL << (L-1)) - 1);
    Result = (1ULL << (L-1)) | R'.
*/

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;                 /* offset is 1; n=0 is undefined, return 0 for safety */
    }

    /* compute bit length L of n */
    idx_t L = 0;
    idx_t t = n;
    while (t) {
        L++;
        t >>= 1;
    }

    idx_t m = L - 1;                         /* length of the remaining bits */
    seq_t msb = (seq_t)1 << m;               /* 1 at position m */
    seq_t rest = (seq_t)n - msb;             /* remaining bits */

    seq_t rest_rot;
    if (m == 0) {
        rest_rot = 0;
    } else if (m == 1) {
        rest_rot = rest;                     /* single bit unchanged */
    } else {
        idx_t k = 2 % m;                     /* rotation amount modulo m */
        rest_rot = ((rest >> k) | (rest << (m - k))) & (((seq_t)1 << m) - 1);
    }

    return msb | rest_rot;
}
