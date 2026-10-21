/*
 * A070561  a(0) = 0; for n > 0, a(n) = (fecundity of n) + 1.
 * https://oeis.org/A070561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070561: a(0)=0; for n>0, a(n)=1+fecundity(n) where fecundity(n)=
   the number of steps (x → x + product_of_digits(x)) until a term
   containing a zero digit appears.  This is equivalent to the number
   of distinct terms obtained from n, f(n), f(f(n)), … until the first
   term that contains a zero digit. */

#include <stdbool.h>

/* Compute the product of the decimal digits of x.
   Returns 0 immediately if a 0 digit is present. */
static seq_t digit_product(seq_t x)
{
    if (x == (seq_t)0) return (seq_t)0;
    seq_t prod = (seq_t)1;
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (d == (seq_t)0) return (seq_t)0;
        prod *= d;
        x /= (seq_t)10;
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* a(0)=0 */

    seq_t cur = (seq_t)n;

    /* If n already contains a zero digit, the chain stops immediately. */
    if (digit_product(cur) == (seq_t)0) return (seq_t)1;  /* a(n)=1 */

    seq_t count = (seq_t)1;          /* count the starting term */

    while (1) {
        seq_t prod = digit_product(cur);
        if (prod == (seq_t)0) break;  /* cur contains a zero digit → stop */
        cur = cur + prod;             /* next term */
        count++;                      /* new distinct term added */
    }

    return count;
}
