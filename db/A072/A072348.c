/*
 * A072348  Number of primes whose decimal representation has the form rns
 *          where r and s are single digits.
 * https://oeis.org/A072348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For each n ≥ 0, a(n) counts the primes of the form * r‖n‖s where
 *            r∈{1,…,9} is a single leading digit, s∈{1,3,7,9} is a * single
 *            trailing digit, and ‖ denotes decimal concatenation. This is the
 *            * “prime sandwich” count
 * Range:     u8 none, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   digit_len, is_prime_big, mul_small, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"     /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/mul_small.h" /* seq_t mul_small(seq_t x, unsigned int m); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    static const idx_t trailing[4] = {1, 3, 7, 9};
    seq_t count = 0;

    /* length of n in decimal digits */
    int len_n = digit_len((seq_t)n);
    /* 10^len(n) as seq_t */
    seq_t pow10_n = pow10((idx_t)len_n);

    for (idx_t r = 1; r <= 9; ++r) {
        /* part = r concatenated with n :  r*10^len(n) + n */
        seq_t part = seq_mul((seq_t)r, pow10_n);
        part = part + (seq_t)n;                /* r‖n‖ */

        for (size_t ti = 0; ti < 4; ++ti) {
            /* candidate = part*10 + s :  r‖n‖s */
            seq_t cand = mul_small(part, 10);
            cand = cand + (seq_t)trailing[ti];

            if (is_prime_big(cand))
                count = count + 1;
        }
    }
    return count;
}
