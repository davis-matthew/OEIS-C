/*
 * A003594  Numbers of the form 3^i*7^j with i, j >= 0.
 * https://oeis.org/A003594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   3-smooth-style merge: a(1) = 1; the sequence is generated in
 *            increasing order by merging {3*a(k)} and {7*a(k)} (two-pointer
 *            method); a(n) = 3^i*7^j for the n-th exponent pair (i, j) in
 *            that order
 * Range:     u8 n <= 12, u16 n <= 38, u32 n <= 132, u64 n <= 492,
 *            u128 n <= 1904, big any n
 * Helpers:   a003594_max, a003594_mul, a003594_pw, a003594_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a003594_max.h"
#include "../utils/a003594_mul.h"
#include "../utils/a003594_pw.h"
#include "../utils/a003594_val.h"

seq_t a(idx_t n) {
    static unsigned short *E3 = (unsigned short *)0;
    static unsigned short *E7 = (unsigned short *)0;
    static idx_t len = 0, cap = 0, i3 = 0, i7 = 0;
    static bool done = false;
    bool ovf;

    if (E3 == (unsigned short *)0) {
        cap = 1024;
        E3 = (unsigned short *)malloc(cap * sizeof(unsigned short));
        E7 = (unsigned short *)malloc(cap * sizeof(unsigned short));
        E3[0] = 0;
        E7[0] = 0;
        len = 1;
    }

    while (len < n && !done) {
        if (len >= cap) {
            cap *= 2;
            E3 = (unsigned short *)realloc(E3, cap * sizeof(unsigned short));
            E7 = (unsigned short *)realloc(E7, cap * sizeof(unsigned short));
        }
        unsigned a3 = (unsigned)E3[i3] + 1, b3 = E7[i3];
        unsigned a7 = E3[i7], b7 = (unsigned)E7[i7] + 1;

        if (a3 == a7 && b3 == b7) {
            E3[len] = (unsigned short)a3;
            E7[len] = (unsigned short)b3;
            len++;
            i3++;
            i7++;
            continue;
        }

        bool o3, o7;
        seq_t v3 = a003594_val(a3, b3, &o3);
        seq_t v7 = a003594_val(a7, b7, &o7);
        bool take3;
        if (o3 && o7) {
            done = true;
            break;
        } else if (o3) {
            take3 = false;
        } else if (o7) {
            take3 = true;
        } else {
            take3 = (v3 < v7);
        }
        if (take3) {
            E3[len] = (unsigned short)a3;
            E7[len] = (unsigned short)b3;
            i3++;
        } else {
            E3[len] = (unsigned short)a7;
            E7[len] = (unsigned short)b7;
            i7++;
        }
        len++;
    }

    if (n > len) return 0;
    return a003594_val(E3[n - 1], E7[n - 1], &ovf);
}
