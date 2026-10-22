/*
 * A006298  Number of genus 2 rooted maps with 1 face with n vertices.
 * https://oeis.org/A006298
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((5*n-2)*(2*n)!)/(1440*n!*(n-4)!) for n >= 4, evaluated
 *            by prime factorization (Legendre's formula) so that every
 *            intermediate product divides the final value
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   legendre_v
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/legendre_v.h"

seq_t a(idx_t n) {
    uint64_t lim, i, j, m;
    unsigned char *comp;
    seq_t r = 1;
    if (n < 4) return 0;
    lim = 2 * (uint64_t)n;
    comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    m = 5 * (uint64_t)n - 2;
    for (i = 2; i <= lim; i++) {
        long long e;
        if (comp[i]) continue;
        for (j = i * i; j <= lim; j += i) comp[j] = 1;
        e = legendre_v(lim, i) - legendre_v((uint64_t)n, i)
            - legendre_v((uint64_t)n - 4, i);
        while (m % i == 0) {
            m /= i;
            e++;
        }
        if (i == 2) e -= 5;
        else if (i == 3) e -= 2;
        else if (i == 5) e -= 1;
        while (e > 0) {
            r *= (seq_t)i;
            e--;
        }
    }
    free(comp);
    if (m > 1) r *= (seq_t)m;
    return r;
}
