/*
 * A100601  Denominator of the best rational approximation to the decimal
 *          representation of the digital roots of m^n, m=1,2,..
 * https://oeis.org/A100601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, easy, nonn
 * Formula:   a(n) = 9, 1001, 50, 333, 1001, 100, 333, 11, 5 repeat with
 *            period 9 (a(n+9) = a(n) for all n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* period is 9: use n modulo 9, mapping 0→9 */
    idx_t r = n % 9;
    if (r == 0) r = 9;

    switch ((int)r) {
        case 1: return (seq_t)9;          /* 1 → 1/9  */
        case 2:            /* 2,5 → 1/1001  */
        case 5: return (seq_t)1001;
        case 3: return (seq_t)50;         /* 3 → 7/50  */
        case 4: case 7: return (seq_t)333;/* 4,7 → 147/999 → 49/333  */
        case 6: return (seq_t)100;        /* 6 → 17/100 */
        case 8: return (seq_t)11;         /* 8 → 18/99 → 2/11  */
        case 9: return (seq_t)5;          /* 9 → 1/5   */
    }
    return (seq_t)0; /* unreachable */
}
