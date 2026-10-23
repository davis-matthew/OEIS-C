/*
 * A167719  Squares that become a prime number when prefixed with a 4.
 * https://oeis.org/A167719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 19, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167719: squares x such that 4·10^k + x is prime
   (with k = number of decimal digits of x).  Computed by brute‑force
   search. */

#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"

/* Count decimal digits of a seq_t number.  Uses only seq_t arithmetic. */
static idx_t decimal_digits(seq_t v)
{
    idx_t d = 0;
    seq_t t = v;
    do {
        t /= (seq_t)10;
        ++d;
    } while (t != 0);
    return d;
}

/* Return the n-th element of the sequence, with offset 1. */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many valid squares have been seen */
    for (idx_t m = 1; ; ++m) {
        seq_t sq = (seq_t)m * (seq_t)m;          /* m² */

        idx_t d = decimal_digits(sq);           /* digits of sq */
        seq_t candidate = ((seq_t)4 * pow10(d)) + sq;

        if (is_prime_big(candidate)) {
            ++found;
            if (found == n)
                return sq;
        }
    }
}
