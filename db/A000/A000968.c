/*
 * A000968  Sum of odd Fermat coefficients rounded to nearest integer.
 * https://oeis.org/A000968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( (L(2n+1) - 2*cos((2n+1)*Pi/3)) / (2*(2n+1)) ),
 *            where L = Lucas numbers (A000032), L(1) = 1, L(2) = 3, L(k) =
 *            L(k-1) + L(k-2). This works because (n:c) = binomial(2n-c,c-1)/c
 *            = L(2n+1,c)/(2n+1), with L(m,c) = m/(m-c)*binomial(m-c,c) the
 *            Lucas-polynomial coefficients, and sum_{odd c}...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 51, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A968_MASK 0xFFFFFFFFULL

/* value = hi * 2^32 + lo, with lo < 2^32 */
static void a968_add(seq_t *rh, uint64_t *rl, seq_t ah, uint64_t al, seq_t bh, uint64_t bl) {
    uint64_t s = al + bl;
    uint64_t carry = s >> 32;
    *rl = s & A968_MASK;
    *rh = ah + bh;
    *rh += (seq_t)carry;
}

seq_t a(idx_t n) {
    idx_t m = 2 * n + 1;
    seq_t ah = 0, bh = 0, ch = 0;
    uint64_t al = 1, bl = 3, cl = 0;   /* L(1) = 1, L(2) = 3 */
    idx_t k;
    uint64_t add, D, s, rh, ql;
    seq_t hi, qh;

    for (k = 3; k <= m; k++) {
        a968_add(&ch, &cl, ah, al, bh, bl);
        ah = bh; al = bl;
        bh = ch; bl = cl;
    }
    /* bh:bl = L(m). 2*cos(m*Pi/3) = -2 if m%6==3, else 1 (m odd).
       numerator = L(m) - 2cos + m */
    if (m % 6 == 3) add = (uint64_t)m + 2;
    else add = (uint64_t)m - 1;

    s = bl + add;
    hi = bh;
    hi += (seq_t)(s >> 32);
    s &= A968_MASK;

    D = 2 * (uint64_t)m;
    qh = hi / D;
    rh = (uint64_t)(hi % D);
    ql = ((rh << 32) + s) / D;

    if (qh == 0) return (seq_t)ql;
    {
        seq_t r = qh;
        r <<= 16;
        r <<= 16;
        r += (seq_t)ql;
        return r;
    }
}
