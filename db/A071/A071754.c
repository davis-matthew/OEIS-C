/*
 * A071754  a(n) = Sum_{k=0..n} pp(k) where pp(k) is the parity of p(k) the
 *          k-th partition number = A040051(k).
 * https://oeis.org/A071754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} pp(k), where pp(k) = parity(p(k)) and p(k)
 *            is the k-th partition number
 * Range:     u8 n <= 463, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state to memoise results between calls */
    static idx_t       max_n = 0;          /* greatest n already computed   */
    static uint8_t    *par   = NULL;       /* parity of p(k) (0 or 1)       */
    static seq_t      *cum   = NULL;       /* cumulative sums a(k)          */

    /* initialise on first call */
    if (!par) {
        max_n = 0;
        par   = (uint8_t*)malloc(sizeof(uint8_t));
        if (!par) return (seq_t)0;          /* allocation failed */
        cum   = (seq_t*)malloc(sizeof(seq_t));
        if (!cum) { free(par); return (seq_t)0; }

        par[0] = 1;                          /* p(0)=1 is odd              */
        cum[0] = (seq_t)1;                   /* a(0)=1                      */
    }

    /* extend tables if a larger n is requested */
    if (n > max_n) {
        size_t new_len = (size_t)(n + 1);
        uint8_t *p_new = (uint8_t*)realloc(par, new_len * sizeof(uint8_t));
        seq_t   *c_new = (seq_t*)realloc(cum, new_len * sizeof(seq_t));
        if (!p_new || !c_new) return (seq_t)0; /* allocation failed */

        par = p_new;
        cum = c_new;

        /* compute parity and cumulative sums for new indices */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            uint8_t val = 0;                  /* parity of p(i)            */
            for (int k = 1; ; ++k) {
                idx_t pent1 = (idx_t)k * (3 * (idx_t)k - 1) / 2;
                if (pent1 > i) break;
                val ^= par[i - pent1];

                idx_t pent2 = (idx_t)k * (3 * (idx_t)k + 1) / 2;
                if (pent2 <= i)
                    val ^= par[i - pent2];
            }
            par[i] = val;
            cum[i] = cum[i - 1] + (seq_t)val;
        }
        max_n = n;
    }

    return cum[n];
}
