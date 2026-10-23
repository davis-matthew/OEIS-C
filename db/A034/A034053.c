/*
 * A034053  Numbers with multiplicative digital root value 6.
 * https://oeis.org/A034053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034053: Numbers with multiplicative digital root 6.
   Computed by brute‑force search: a(n) is the nth positive integer
   whose multiplicative digital root equals 6. */

#include <stdint.h>
#include <stdlib.h>

/* Compute the product of the decimal digits of x. */
static seq_t digit_product(seq_t x)
{
    seq_t prod = 1;
    while (x > 0) {
        prod *= (x % 10);
        x /= 10;
    }
    return prod;
}

/* Return the multiplicative digital root of x. */
static seq_t mult_digit_root(seq_t x)
{
    while (x >= (seq_t)10) {
        x = digit_product(x);
    }
    return x;
}

/* Return the nth term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset handling – not used for n ≥ 1 */

    seq_t cand   = 0;              /* candidate integer */
    idx_t found  = 0;              /* how many terms found so far */

    for (;;) {
        cand += 1;
        if (mult_digit_root(cand) == (seq_t)6) {
            ++found;
            if (found == n) {
                return cand;
            }
        }
    }
}
