/*
 * A188189  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(5), k=2, [ ]=floor.
 * https://oeis.org/A188189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(5)) - floor(2*sqrt(5)) -
 *            floor((n-2)*sqrt(5))
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: returns floor(sqrt(x)) for an unsigned integer type seq_t. */
static seq_t
floor_sqrt_seq(seq_t x)
{
    if (x < 2)
        return x;
    seq_t low  = 1;
    seq_t high = x;
    seq_t mid, mid2;
    while (low <= high)
    {
        mid  = low + ((high - low) >> 1);   /* mid = (low+high)/2  */
        mid2 = mid * mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;          /* high is the floor of sqrt(x) */
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    seq_t N = n;                        /* convert index to seq_t */
    seq_t f1 = floor_sqrt_seq((seq_t)5 * N * N);   /* floor(n*sqrt5)   */
    seq_t f2 = 4;                        /* floor(2*sqrt5)=4 */

    long long f3;                         /* handle possible negative floor */
    if (n >= 2) {
        seq_t M = N - (seq_t)2;
        f3 = (long long)floor_sqrt_seq((seq_t)5 * M * M);
    } else /* n == 1 */ {
        seq_t pos = (seq_t)2 - N;         /* positive magnitude of (n-2) */
        seq_t tmp = floor_sqrt_seq((seq_t)5 * pos * pos);
        f3 = -((long long)tmp + 1);       /* floor((n-2)*sqrt5) for negative argument */
    }

    long long result = (long long)f1 - (long long)f2 - f3;
    return (seq_t)result;
}
