/*
 * A054428  Inverse permutation to A054427.
 * https://oeis.org/A054428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k such that A054427(k) = n; the bits of n-1 give
 *            the Stern-Brocot path p/q (rebuilt by reverse Euclid), then k =
 *            sum_{j<p} phi(j) + #{d<=q : gcd(p,d)=1}
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t A054428_gcd(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    idx_t m = n - 1;
    idx_t p = 1, q = 1;
    while (m > 0) {
        if (m & 1) p += q;
        else q += p;
        m >>= 1;
    }
    /* count coprime pairs (num, den<=num) with num < p, via totient sieve */
    seq_t total = 0;
    if (p > 1) {
        idx_t lim = p - 1;
        idx_t *phi = (idx_t *)malloc((size_t)(lim + 1) * sizeof(idx_t));
        idx_t i, j;
        for (i = 0; i <= lim; i++) phi[i] = i;
        for (i = 2; i <= lim; i++) {
            if (phi[i] == i) {
                for (j = i; j <= lim; j += i) phi[j] -= phi[j] / i;
            }
        }
        for (i = 1; i <= lim; i++) total += (seq_t)phi[i];
        free(phi);
    }
    for (idx_t d = 1; d <= q; d++) {
        if (A054428_gcd(p, d) == 1) total++;
    }
    return total;
}
