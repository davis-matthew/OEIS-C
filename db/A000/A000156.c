/*
 * A000156  Number of ways of writing n as a sum of 24 squares.
 * https://oeis.org/A000156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^4*(sigma_11(n) - 2*sigma_11(n/2) + 2^12*sigma_11(n/4))
 *            - 2^7*259*(-1)^n*tau(n) - 2^16*tau(n/2))/691, n >= 1, a(0) = 1,
 *            where sigma_11(x) = tau(x) = 0 for non-integer x and tau is
 *            Ramanujan's tau, computed by tau(m) = m^4*sigma(m) -
 *            24*Sum_{i=1..m-1} i^2*(35*i^2 - 52*i*m + 18*m^2)*sigma(...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 79, u128 n <= 4482,
 *            big any n
 * Helpers:   add_signed, ramanujan_tau, sigma11, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/add_signed.h"
#include "../utils/ramanujan_tau.h"
#include "../utils/sigma11.h"
#include "../utils/u128_to_seq.h"

seq_t a(idx_t n)
{
    static int mode = -1; /* 1 = fixed width, 0 = arbitrary precision */
    seq_t pos = 0, neg = 0, diff;
    __int128 t;

    if (n == 0) return 1;

    if (mode < 0) {
        seq_t x = 1;
        int k;
        for (k = 0; k < 200; k++) x = x * 2ULL;
        mode = (x == 0) ? 1 : 0;
    }

    pos += sigma11(n) * 16ULL;
    if (n % 2 == 0) neg += sigma11(n / 2) * 32ULL;
    if (n % 4 == 0) pos += sigma11(n / 4) * 65536ULL;

    t = ramanujan_tau(n);
    if (n % 2 == 1) add_signed(&pos, &neg, t, 33152ULL);
    else add_signed(&pos, &neg, -t, 33152ULL);

    if (n % 2 == 0) {
        t = ramanujan_tau(n / 2);
        add_signed(&pos, &neg, -t, 65536ULL);
    }

    diff = pos - neg;

    if (mode == 0) return diff / 691;

    {
        /* exact division by 691 modulo 2^width via inverse of 691 mod 2^128 */
        unsigned __int128 inv = 691;
        int k;
        seq_t r = 0;
        for (k = 0; k < 8; k++) inv = inv * ((unsigned __int128)2 - (unsigned __int128)691 * inv);
        for (k = 3; k >= 0; k--) {
            r = r * 65536ULL;
            r = r * 65536ULL;
            r += diff * (uint64_t)(uint32_t)(inv >> (32 * k));
        }
        return r;
    }
}
