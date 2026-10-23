/*
 * A116544  Triangular numbers for which the multiplicative digital root is
 *          also a triangular number.
 * https://oeis.org/A116544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 312, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Triangular numbers whose multiplicative digital
 * root is also a triangular number (OEIS A116544).  The function
 * returns the n‑th such number, where n is 1‑based. */

#include <stdbool.h>

/* Compute the multiplicative digital root of x.
 * For x < 10 the algorithm returns x itself. */
static seq_t mul_digital_root(seq_t x)
{
    while (x >= 10) {
        seq_t prod = 1;
        while (x > 0) {
            seq_t d = x % 10;
            if (d == 0) {
                prod = 0;       /* a zero digit makes the product zero */
                x = 0;          /* remaining digits are irrelevant */
                break;
            }
            prod *= d;
            x /= 10;
        }
        x = prod;
    }
    return x;
}

/* Return the n‑th triangular number whose multiplicative digital
 * root is also a triangular number.  n is 1‑based.   */
seq_t a(idx_t n)
{
    idx_t   count = 0;      /* number of valid triangulars found  */
    idx_t   k     = 0;      /* triangle index (k=0 gives T_0=0)      */
    seq_t   T     = 0;      /* current triangular number            */

    while (1) {
        seq_t mdr = mul_digital_root(T);
        /* Triangular digits are 0, 1, 3, 6. */
        if (mdr == 0 || mdr == 1 || mdr == 3 || mdr == 6) {
            ++count;
            if (count == n)
                return T;
        }

        /* next triangular number: T_{k+1} = T_k + (k+1) */
        ++k;
        T += (seq_t)k;
    }
}
