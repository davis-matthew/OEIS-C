/*
 * A101679  A bisection of A101678.
 * https://oeis.org/A101679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=sum{k=0..n,
 *            (-1)^A010060(2n-2k)*mod(binomial(2n-k,k),2)*10^k}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Thue-Morse parity: returns 1 if the number of 1s in x is odd, else 0 */
static inline int popcnt_parity(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x) & 1;
#else
    int r = 0;
    while (x) { r ^= 1; x &= x - 1; }
    return r;
#endif
}

seq_t a(idx_t n)
{
    seq_t pos = 0;            /* sum of terms with positive sign   */
    seq_t neg = 0;            /* sum of terms with negative sign   */
    seq_t power10 = 1;        /* 10^k ; k starts at 0              */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* m = 2n-2k = 2*(n-k) */
        idx_t m = 2ULL * (n - k);

        /* binomial(2n-k,k) is odd iff (k & (2n-2k)) == 0 */
        if ((k & m) == 0)
        {
            seq_t term = power10;   /* 10^k */

            if (popcnt_parity(m) == 0) /* even popcount -> sign +1 */
                pos += term;
            else                       /* odd popcount -> sign -1 */
                neg += term;
        }

        power10 *= 10; /* next power of ten */
    }

    /* pos ≥ neg for all n, so unsigned subtraction yields the correct result */
    return pos - neg;
}
