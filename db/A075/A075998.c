/*
 * A075998  Product{<n/k>: k=1,2,...,n}, where <x> denotes the integer second
 *          nearest to x. (For x=(2h+1)/2, <x> is defined to be h, not h+1; if
 *          x is an integer, then <x> is defined to be x+1.)
 * https://oeis.org/A075998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A075998:
   a(n) = ∏_{k=1..n} <n/k> ,
   where <x> is the integer second‑nearest to x.
   Rules for <x>:
     - if x is an integer, <x> = x + 1;
     - if the fractional part of x is < 0.5, <x> = ceil(x);
     - if the fractional part of x is > 0.5, the integer second‑nearest is floor(x);
     - for a tie (fractional part = 0.5), the lower integer is taken.
   The implementation uses only integer arithmetic and works for any
   seq_t/idx_t types, with or without arbitrary‑precision support. */
seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t q = n / k;          /* integer part of n/k   */
        idx_t r = n % k;          /* remainder              */

        seq_t term;
        /* If n is a multiple of k, or the fractional part of n/k is < 0.5,
           the second‑nearest integer is ceil(n/k) = q+1.
           Otherwise it is floor(n/k) = q (this also covers the tie
           fractional part = 0.5 and the integer case). */
        if (r == 0 || (2 * r < k))
            term = (seq_t)(q + 1);
        else
            term = (seq_t)q;

        prod *= term;
    }
    return prod;
}
