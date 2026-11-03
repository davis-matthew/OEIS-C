/*
 * A055986  a(n+1) = a(n) converted to base 10 from base 15.
 * https://oeis.org/A055986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0)=10, a(n+1)=interpret a(n) decimal digits as base‑15 and
 *            convert to decimal
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 17, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t convert_base15(seq_t v)
{
    if (v == 0) return (seq_t)0;
    seq_t tmp = v;
    size_t cnt = 0;
    while (tmp != 0) { tmp /= (seq_t)10; cnt++; }
    unsigned char *digits = (unsigned char*)malloc(cnt);
    if (!digits) return (seq_t)0;
    tmp = v;
    for (size_t i = 0; i < cnt; ++i) {
        digits[i] = (unsigned char)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    seq_t res = (seq_t)0;
    for (size_t i = cnt; i > 0; --i) {
        res = res * (seq_t)15 + (seq_t)digits[i-1];
    }
    free(digits);
    return res;
}

seq_t a(idx_t n)
{
    seq_t v = (seq_t)10;
    for (idx_t i = 0; i < n; ++i) {
        v = convert_base15(v);
    }
    return v;
}
