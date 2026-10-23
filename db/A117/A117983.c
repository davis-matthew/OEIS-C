/*
 * A117983  A modified Legendre-binomial transform of 3^n for p=3.
 * https://oeis.org/A117983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * L(C(n,k)/3) * 3^k where L(j/3)
 *            is the Legendre symbol modulo 3. (C(n,k) is the binomial
 *            coefficient.)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used in helper types */

static inline idx_t binom_mod3_lucas(idx_t n, idx_t k)
{
    idx_t res = 1;
    while (n || k) {
        idx_t ni = n % 3;
        idx_t ki = k % 3;
        if (ki > ni)            /* binomial equals 0 modulo 3 */
            return 0;
        idx_t c;                /* binomial of a single base‑3 digit */
        if (ni == 0)            /* ki must be 0 */
            c = 1;
        else if (ni == 1)       /* C(1,0)=C(1,1)=1 */
            c = 1;
        else {                  /* ni == 2 */
            if (ki == 0)        /* C(2,0)=1 */
                c = 1;
            else if (ki == 1)   /* C(2,1)=2 */
                c = 2;
            else                /* ki == 2 -> C(2,2)=1 */
                c = 1;
        }
        res = (res * c) % 3;    /* keep the value in {0,1,2} */
        n /= 3;
        k /= 3;
    }
    return res;                  /* 0,1,or 2 */
}

seq_t a(idx_t n)
{
    seq_t pos = 0;         /* positive contributions */
    seq_t neg = 0;         /* negative contributions */
    seq_t pow3k = 1;       /* 3^k for current k */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t binom_mod3 = binom_mod3_lucas(n, k);
        int leg;  /* Legendre symbol: 0, 1, or -1 */
        if (binom_mod3 == 0)
            leg = 0;
        else if (binom_mod3 == 1)
            leg = 1;
        else            /* binom_mod3 == 2 */
            leg = -1;

        /* overall sign: (-1)^(n-k) * leg */
        int sign = ((n - k) & 1) ? -1 : 1;
        sign *= leg;

        if (sign > 0)
            pos += pow3k;
        else if (sign < 0)
            neg += pow3k;
        /* if sign == 0, nothing to add */

        pow3k *= (seq_t)3;
    }

    /* pos >= neg for all n where the value fits in seq_t */
    return pos - neg;
}
