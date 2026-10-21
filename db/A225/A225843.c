/*
 * A225843  Integral parts of sums of the reciprocals of the Collatz (3x+1)
 *          sequence starting with n.
 * https://oeis.org/A225843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ⌊∑_{k=1}^{τ(n)} 1/Collatz_k(n)⌋,
 * where Collatz_1(n)=n and for k≥1,
 *   Collatz_{k+1}(n)=
 *       Collatz_k(n)/2          if Collatz_k(n) is even,
 *       3·Collatz_k(n)+1       if Collatz_k(n) is odd.
 * τ(n) is the total number of terms until the first 1 is reached
 * (including the starting n), i.e. A006577(n).
 * The sum is performed with double precision; the final integer part
 * is obtained by truncating to a native long long, then converted to
 * seq_t.
 */

#include <stdint.h>
#include <math.h>

/* External linkage (do not make static) */
seq_t a(idx_t n)
{
    /* According to the OEIS offset, a(1) is the first defined term.
       For n < 1 the behaviour is irrelevant; we return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Use double-precision sum.  The result is always 1, 2 or 3,
       so rounding errors are negligible.                   */
    long double sum = 0.0L;

    /* Current value in the Collatz trajectory. */
    uint64_t cur = (uint64_t)n;

    /* Accumulate reciprocals until the first 1 is encountered. */
    for (;;) {
        sum += 1.0L / (long double)cur;
        if (cur == 1) break;
        if ((cur & 1ULL) == 0ULL)
            cur >>= 1;           /* even: divide by 2 */
        else
            cur = cur * 3ULL + 1ULL; /* odd: 3*n+1 */
    }

    /* Floor the accumulated sum.  Add a tiny epsilon to guard against
       the unlikely event that the true sum is an integer but the
       floating‑point representation is just below it. */
    long long val = (long long)(floor(sum + 1e-12L));
    return (seq_t)val;
}
