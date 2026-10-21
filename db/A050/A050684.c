/*
 * A050684  Number of nonzero palindromes < 10^n and containing at least one
 *          digit '1'.
 * https://oeis.org/A050684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   pow10, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050684: Number of nonzero palindromes < 10^n and containing at least one digit '1'.
   Formula: a(2n)   = 2*(10^n - 9^n)
            a(2n+1) = 2*(10^n - 9^n) + 9*10^n - 8*9^n
   (n ≥ 1)                                                                   */

#include "../utils/pow10.h"   /* pow10(idx_t e) -> seq_t */
#include "../utils/pow_seq.h" /* pow_seq(seq_t a, int k) -> seq_t */

seq_t a(idx_t n)
{
    /* integer division by 2 gives floor(n / 2) = ⌊n/2⌋ */
    idx_t k = n >> 1;                     /* k = n/2 (floor)             */

    /* 10^k and 9^k in seq_t */
    seq_t pow10k = pow10(k);
    seq_t pow9k  = pow_seq((seq_t)9, (int)k);

    /* diff = 10^k - 9^k */
    seq_t diff = pow10k - pow9k;

    if ((n & 1) == 0) {                  /* n even: a(n) = 2*diff */
        return diff * 2;
    }

    /* n odd: a(n) = 2*diff + (9*10^k - 8*9^k) */
    seq_t term = 9 * pow10k - 8 * pow9k;
    return diff * 2 + term;
}
