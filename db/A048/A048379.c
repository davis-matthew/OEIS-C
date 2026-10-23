/*
 * A048379  Apply the transformation 0->1->2->3->4->5->6->7->8->9->0 to digits
 *          of n.
 * https://oeis.org/A048379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = each decimal digit d of n is replaced by (d+1) mod 10
 * Range:     u8 n <= 144, u16 n <= 9796, u32 n <= 9796, u64 n <= 9796,
 *            u128 n <= 9796, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A048379:
   a(0) = 1, a(1) = 2, a(10) = 21, ...
   The function processes the decimal representation of n, increments
   each digit modulo 10, and rebuilds the resulting number. */
seq_t a(idx_t n)
{
    /* Special case: n = 0 gives a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t result  = 0;
    seq_t place   = 1;      /* 10^position */
    idx_t x       = n;      /* work on a copy of n */

    while (x > 0) {
        unsigned digit   = (unsigned)(x % 10);      /* current decimal digit */
        unsigned ndigit  = (digit + 1) % 10;        /* (d+1) mod 10 */
        result += (seq_t)ndigit * place;           /* add new digit at current position */
        place  *= 10;                               /* move to next higher position */
        x      /= 10;                               /* drop processed digit */
    }

    return result;
}
