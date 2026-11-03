/*
 * A101650  A Thue-Morse-Stern sequence.
 * https://oeis.org/A101650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (A002487(n+1) - 1) mod 3 where A002487 is Stern's
 *            diatomic sequence (a(2n) = a(n), a(2n+1) = a(n)+a(n+1), a(0)=0,
 *            a(1)=1)
 * Range:     u8 n <= 16382, u16 n <= 16382, u32 n <= 16382, u64 n <= 16382,
 *            u128 n <= 16382, big any n
 * Validated: standard, pass, 117 terms through n = 9657 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t stern_mod3(idx_t n)
{
    /* Returns s(n) mod 3, where s is Stern's diatomic sequence. */
    if (n == 0) return 0;
    seq_t a = 1, b = 1;                /* s(1)=1, s(2)=1 */
    int msb = 63;
    while (msb >= 0 && ((n >> msb) & 1ULL) == 0) --msb; /* find highest set bit */
    if (msb == 0) return a;            /* n == 1 */
    for (int i = msb - 1; i >= 0; --i) {
        if ((n >> i) & 1ULL) {
            a = (a + b) % 3;          /* bit 1: new (a,b) = (a+b,b) */
        } else {
            b = (b + a) % 3;          /* bit 0: new (a,b) = (a,a+b) */
        }
    }
    return a;                          /* s(n) mod 3 */
}

seq_t a(idx_t n)
{
    idx_t m = n + 1;                   /* use A002487(n+1) */
    seq_t s = stern_mod3(m);           /* s(n+1) mod 3 */
    return (s + 2) % 3;                /* (−1 + s) mod 3 */
}
