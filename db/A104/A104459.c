/*
 * A104459  Possible differences between adjacent palindromes.
 * https://oeis.org/A104459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A104459: Possible differences between adjacent palindromes.
 * Formula: a(1)=1, a(2)=2, a(3)=10, a(4)=11,
 *          a(n)=10*a(n-2) for n≥5 (odd and even indices separated)
 */
#include <stdint.h>

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n % 2 == 1) {
        idx_t exp = (n - 1) / 2;
        return pow10(exp);
    } else {
        idx_t exp = (n - 4) / 2;
        seq_t val = pow10(exp);
        return val * 11U;
    }
}
