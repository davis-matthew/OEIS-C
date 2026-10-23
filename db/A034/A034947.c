/*
 * A034947  Jacobi (or Kronecker) symbol (-1/n).
 * https://oeis.org/A034947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice, easy, mult
 * Formula:   a(n) = Jacobi(-1/n) = (-1)^(# of prime factors ≡3 mod 4 counted
 *            with multiplicity). Equivalently, a(2n)=a(n), a(4k+1)=1,
 *            a(4k+3)=-1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Strip all factors of 2 – a(2^e)=1 and a(2n)=a(n) */
    while ((n & 1ULL) == 0ULL)
        n >>= 1;

    /* Now n is odd.  For odd n, Jacobi(-1/n)=1 iff n≡1 (mod 4), otherwise -1. */
    if ((n % 4ULL) == 1ULL)
        return (seq_t)1;
    else
        return (seq_t)-1;
}
