/*
 * A092366  Coefficient of x^n in expansion of (1+n*x+n*x^2)^n.
 * https://oeis.org/A092366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=⌈n/2⌉}^{n} n^k · C(n,k) · C(k,n-k) (coefficient of
 *            x^n in (1+n·x+n·x^2)^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* the first k contributing term is ceil(n/2) */
    idx_t start = (n + 1) / 2;

    /* current binomial coefficients and power of n */
    seq_t binom_nk   = binomial(n, start);              /* C(n,start)          */
    seq_t binom_kkm  = binomial(start, n - start);      /* C(start,n-start)    */
    seq_t pow_nk     = 1;                                /* n^start             */
    for (idx_t i = 0; i < start; ++i)
        pow_nk *= (seq_t)n;

    seq_t result = 0;
    idx_t    k    = start;
    idx_t   m    = n - start;   /* m = n - k */

    for (;; ++k) {
        /* add current term */
        result += pow_nk * binom_nk * binom_kkm;

        if (k == n)            /* last term reached */
            break;

        /* ------- update C(n,k) -> C(n,k+1) ------- */
        seq_t tmp = binom_nk * (seq_t)(n - k);         /* binom_nk * (n-k) */
        binom_nk = tmp / (seq_t)(k + 1);               /* π (n-k)/(k+1) */

        /* ------- update C(k,n-k) -> C(k+1,n-(k+1)) ------- */
        /* B_{k+1} = B_k * m/(2k-n+1) * (k+1)/(2k-n+2)  (exact integer division) */
        tmp = binom_kkm * (seq_t)m;                   /* * m                */
        tmp = tmp / (seq_t)(2 * k - n + 1);           /* / (2k-n+1)         */
        tmp = tmp * (seq_t)(k + 1);                   /* * (k+1)            */
        binom_kkm = tmp / (seq_t)(2 * k - n + 2);     /* / (2k-n+2)         */

        --m;      /* new m = n-(k+1) */

        /* ------- update power n^k -> n^(k+1) ------- */
        pow_nk *= (seq_t)n;
    }

    return result;
}
