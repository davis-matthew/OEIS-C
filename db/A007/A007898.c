/*
 * A007898  a(n) = psi_c(n), where Product_{k>1} 1/(1-1/k^s)^A007897(k) =
 *          Sum_{k>0} psi_c(k)/k^s.
 * https://oeis.org/A007898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Dirichlet Euler transform: Product_{k>1} 1/(1-1/k^s)^b(k) = Sum
 *            a(n)/n^s, where b = A007897 is multiplicative with b(p^e) =
 *            (phi(p^e) + #{x mod p^e : x^2 = 1})/2, i.e. b(p^e) =
 *            (phi(p^e)+2)/2 for odd p, b(2)=1, b(4)=2, b(2^e) = 2^(e-2)+2 for
 *            e>=3
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   alloc_zero, b_of, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/b_of.h"

static seq_t *tab = NULL;
static idx_t cap = 0;

static void build(idx_t N) {
    seq_t *f = alloc_zero(N + 1);
    f[1] = 1;
    for (idx_t k = 2; k <= N; k++) {
        idx_t b = b_of(k);
        for (idx_t j = N / k; j >= 1; j--) {
            idx_t m = j * k;
            idx_t q = j, t = 1;
            seq_t c = 1;
            for (;;) {
                c = c * (seq_t)(b + t - 1) / (seq_t)t;
                f[m] += f[q] * c;
                if (q % k != 0) break;
                q /= k;
                t++;
            }
        }
    }
    tab = f;
    cap = N;
}

seq_t a(idx_t n) {
    if (n > cap) {
        idx_t N = n;
        if (N < 2 * cap) N = 2 * cap;
        if (N < 1024) N = 1024;
        if (tab != NULL) release(tab, cap + 1);
        tab = NULL;
        build(N);
    }
    return tab[n];
}
