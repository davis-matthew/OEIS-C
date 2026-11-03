/*
 * A103978  Expansion of (sqrt(1-12*x^2)+12*x^2+2*x-1)/(2*x*sqrt(1-12*x^2)).
 * https://oeis.org/A103978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For n even let m = n/2 : a(n) = Catalan(m) * (m + 1) * 3^m For n
 *            odd let m = (n-1)/2 : a(n) = Catalan(m) * 3^(m+1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1827 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Catalan number C(k) = binom(2k,k)/(k+1) computed multiplicatively */
static seq_t catalan(idx_t k)
{
    seq_t res = (seq_t)1;
    /* compute binom(2k,k) */
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(k + i) / (seq_t)i;
    }
    /* divide by (k+1) to get Catalan number */
    res /= (seq_t)(k + 1);
    return res;
}

seq_t a(idx_t n) {
    if ((n & 1U) == 0) {                     /* n even */
        idx_t m = n >> 1;
        seq_t cat = catalan(m);
        seq_t pw  = ipow_idx((seq_t)3, m);
        return cat * ((seq_t)(m + 1)) * pw;
    } else {                                 /* n odd */
        idx_t m = (n - 1) >> 1;
        seq_t cat = catalan(m);
        seq_t pw  = ipow_idx((seq_t)3, m + 1);
        return cat * pw;
    }
}
