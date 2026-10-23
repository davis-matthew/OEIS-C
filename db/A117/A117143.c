/*
 * A117143  Number of partitions of n in which any two parts differ by at most
 *          3.
 * https://oeis.org/A117143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (2*floor((n+2)/3)*(14*floor((n+2)/3)^2-(10*n+21)*floor((n+2)/3)+2*(n^2+5*n+7))-(1-(-1)^floor((n+2)/3))*(-1)^(n+2-floor((n+2)/3)))/16
 * Range:     u8 n <= 7, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* partition count where any two parts differ by at most 3 */
    if (n == 0) return 0;          /* not needed for OEIS offset, but harmless */

    /* t = floor((n+2)/3) */
    idx_t t_idx = (n + 2) / 3;
    seq_t t = (seq_t)t_idx;

    seq_t n_seq = (seq_t)n;

    seq_t t_sq = t * t;
    seq_t part1 = 14 * t_sq;                      /* 14*t^2 */
    seq_t part2 = (10 * n_seq + 21) * t;          /* (10*n+21)*t */
    seq_t part3 = 2 * (n_seq * n_seq + 5 * n_seq + 7); /* 2*(n^2+5*n+7) */
    seq_t A = part1 - part2 + part3;              /* the inner bracket */

    seq_t term1 = 2 * t * A;                     /* 2*t*A */

    /* compute the small correction term */
    int term2_signed = 0;
    if ((t_idx & 1) != 0) {                      /* t odd → (1-(-1)^t)=2 */
        idx_t diff = n + 2 - t_idx;              /* n+2-t */
        int s2 = (diff & 1) ? -1 : 1;           /* (-1)^(diff) */
        term2_signed = 2 * s2;                  /* 2*(-1)^(diff) */
    }

    seq_t numerator;
    if (term2_signed == 0) {
        numerator = term1;
    } else if (term2_signed > 0) {
        numerator = term1 - (seq_t)term2_signed;
    } else {
        numerator = term1 + (seq_t)(-term2_signed);
    }

    return numerator / 16;                       /* division is exact */
}
