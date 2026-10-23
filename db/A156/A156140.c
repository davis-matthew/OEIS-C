/*
 * A156140  Accumulation of Stern's diatomic series: a(0)=-1, a(1)=0, and
 *          a(n+1) = (2e(n)+1)*a(n) - a(n-1) for n > 1, where e(n) is the
 *          highest power of 2 dividing n.
 * https://oeis.org/A156140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, easy
 * Formula:   a(0) = -1, a(1) = 0. For n > 1: a(n) = (2*e(n-1)+1)*a(n-1) -
 *            a(n-2), where e(m) is the exponent of the highest power of 2
 *            dividing m (i.e. the number of trailing zero bits of m)
 * Range:     i8 n <= 84, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count trailing zeros of a positive idx_t. */
static idx_t trailing_zeros(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    /* gcc/clang provides a fast builtin. */
    return __builtin_ctzll(n);
#else
    /* Portable fallback implementation. */
    idx_t e = 0;
    while ((n & 1) == 0)
    {
        ++e;
        n >>= 1;
    }
    return e;
#endif
}

/* Accumulation of Stern's diatomic series (OEIS A156140). */
seq_t a(idx_t n)
{
    /* Handle the first two values directly. */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)0;

    /* a_nm2 = a(k-2), a_nm1 = a(k-1). The loop will compute a(k). */
    seq_t a_nm2 = (seq_t)-1;   /* a(0)   */
    seq_t a_nm1 = (seq_t)0;    /* a(1)   */
    seq_t a_n   = a_nm1;       /* placeholder */

    for (idx_t k = 2; k <= n; ++k)
    {
        /* e(k-1) = exponent of 2 in (k-1). */
        idx_t e = trailing_zeros(k - 1);
        seq_t coeff = (seq_t)(2 * e + 1);

        a_n = coeff * a_nm1 - a_nm2;

        /* Advance the sliding window. */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
