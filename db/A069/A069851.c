/*
 * A069851  Number of distinct primes obtained by inserting a 0 at all
 *          possible places in n. This includes prefixing.
 * https://oeis.org/A069851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069851: a(n) = number of distinct primes obtained by inserting a
 * 0 in every possible decimal position of the integer n (including
 * prefixing). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digits.h"     /* int digits(idx_t); */
#include "../utils/pow10.h"     /* seq_t pow10(idx_t); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t); */

seq_t a(idx_t n)
{
    if (n < 1)            /* offset is 1; below 1 is irrelevant */
        return 0;

    /* Number of decimal digits of n */
    idx_t len = digits(n);          /* digits(n) returns int, assigned to idx_t */

    /* At most len+1 insertion positions */
    idx_t pos_count = len + 1;
    seq_t primes[pos_count];
    size_t uniq = 0;

    /* For each position, build the number with a zero inserted */
    for (idx_t pos = 0; pos <= len; ++pos) {
        idx_t digits_after = len - pos;          /* number of digits to the right of the insertion point */

        seq_t pow_shift = pow10(digits_after + 1);   /* 10^(digits_after+1) */
        seq_t pow_after = pow10(digits_after);       /* 10^digits_after */

        /* Split n into left and right parts */
        seq_t left  = (seq_t)n / pow_after;
        seq_t right = (seq_t)n % pow_after;

        /* Construct the new number: left part + 0 + right part */
        seq_t new_val = left * pow_shift + right;

        /* If it is a prime, count it if not already counted */
        if (is_prime_big(new_val)) {
            bool seen = false;
            for (size_t k = 0; k < uniq; ++k) {
                if (primes[k] == new_val) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                primes[uniq++] = new_val;
            }
        }
    }

    return (seq_t)uniq;
}
