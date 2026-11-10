/*
 * A071884  Trajectory of 37 under map x -> A002487(x)*A002487(x+1).
 * https://oeis.org/A071884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 37; a(n+1) = A002487(a(n))*A002487(a(n)+1), where A002487
 *            is Stern's diatomic series (a(0)=0, a(1)=1, a(2k)=a(k),
 *            a(2k+1)=a(k)+a(k+1))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 20, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes s(x) and s(x+1) of Stern's diatomic series by walking the bits of x. */
static void A071884_stern_pair(seq_t x, seq_t *s0, seq_t *s1) {
    seq_t p = 0, q = 1; /* s(m), s(m+1) for m = 0 */
    int nb = 0;
    seq_t t = x;
    while (t > 0) { t >>= 1; nb++; }
    for (int i = nb - 1; i >= 0; i--) {
        seq_t bit = (x >> i) & 1;
        if (bit == 0) {
            /* m -> 2m: s(2m)=s(m), s(2m+1)=s(m)+s(m+1) */
            q = p + q;
        } else {
            /* m -> 2m+1: s(2m+1)=s(m)+s(m+1), s(2m+2)=s(m+1) */
            p = p + q;
        }
    }
    *s0 = p;
    *s1 = q;
}

seq_t a(idx_t n) {
    seq_t x = 37;
    for (idx_t i = 0; i < n; i++) {
        seq_t s0, s1;
        A071884_stern_pair(x, &s0, &s1);
        x = s0 * s1;
    }
    return x;
}
