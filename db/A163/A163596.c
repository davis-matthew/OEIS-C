/*
 * A163596  Smallest cube that contains the decimal digits of n in its exact
 *          middle.
 * https://oeis.org/A163596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest cube x such that x contains the decimal digits
 *            of n in its exact middle. (brute‑force search)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"      /* pow10(idx_t e) */

static int digit_len_seq(seq_t x)
{
    int len = 0;
    if (x == 0) return 1;
    while (x > 0) {
        x /= 10;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    int L = digit_len_seq(target);          /* length of n */

    idx_t m = 0;
    for (;; ++m) {
        seq_t m_seq = (seq_t)m;
        seq_t cube = m_seq * m_seq * m_seq;

        int D = digit_len_seq(cube);        /* length of cube */
        if (D < L) continue;                /* cube too short */

        int diff = D - L;
        if (diff % 2 != 0) continue;        /* middle must be integral */

        int k = diff / 2;                   /* digits on each side */

        seq_t powk = pow10((idx_t)k);
        seq_t powL = pow10((idx_t)L);

        seq_t middle = (cube / powk) % powL;
        if (middle == target) return cube;
    }
}
