/*
 * A085879  Smallest n-th power k^n == 1 (mod 10), where k>1.
 * https://oeis.org/A085879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3^n          if n ≡ 0 mod 4
 *             a(n) = 9^n          if n ≡ 2 mod 4
 *             a(n) = 11^n         if n ≡ 1 or 3 mod 4
 *          (a(1) is the first listed term).
 *
 * The function implements this rule using a binary‑exponentiation helper
 * that works for any seq_t type (fixed‑size integer or arbitrary‐precision).
 */
#include <stdbool.h>

/* local fast power for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;   /* identity element */
    seq_t b = base;
    while (exp != 0) {
        if (exp & 1ULL) {
            result = result * b;
        }
        exp >>= 1ULL;
        if (exp != 0) {
            b = b * b;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;        /* unused due to offset 1 */

    seq_t base;
    switch (n & 3ULL) {                /* n % 4 */
        case 0ULL: base = (seq_t)3; break;
        case 2ULL: base = (seq_t)9; break;
        case 1ULL:          /* fall through */
        case 3ULL: base = (seq_t)11; break;
    }
    return pow_seq_t(base, n);
}
