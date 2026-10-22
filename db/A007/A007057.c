/*
 * A007057  Let S denote the palindromes in the language {0,1,2,3}*; a(n) =
 *          number of words of length n in the language SS.
 * https://oeis.org/A007057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A187274(n) - Sum_{d|n,d<n} phi(n/d)*a(d), a(0) = 1, where
 *            A187274(n) = Sum_{i=0..n-1} 4^ceil(i/2)*4^ceil((n-i)/2) =
 *            (5*n/2)*2^n for even n, n*2^(n+1) for odd n
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   a007057_A187274, a007057_phi, a007057_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a007057_A187274.h"
#include "../utils/a007057_phi.h"
#include "../utils/a007057_pow2.h"

#define A007057_MAXD 4096

seq_t a(idx_t n) {
    static idx_t small[A007057_MAXD];
    static idx_t large[A007057_MAXD];
    static idx_t divs[2 * A007057_MAXD];
    seq_t vals[2 * A007057_MAXD];
    int ns = 0, nl = 0, nd = 0;
    int i, j, k;
    idx_t q;

    if (n == 0) return 1;

    for (q = 1; q * q <= n; q++) {
        if (n % q == 0) {
            small[ns++] = q;
            if (q != n / q) large[nl++] = n / q;
        }
    }
    for (i = 0; i < ns; i++) divs[nd++] = small[i];
    for (i = nl - 1; i >= 0; i--) divs[nd++] = large[i];

    for (k = 0; k < nd; k++) {
        idx_t dk = divs[k];
        seq_t s = 0;
        for (j = 0; j < k; j++) {
            idx_t dj = divs[j];
            if (dk % dj == 0) {
                s += (seq_t)a007057_phi(dk / dj) * vals[j];
            }
        }
        vals[k] = a007057_A187274(dk) - s;
    }
    return vals[nd - 1];
}
