/*
 * A000908  Atom-rooted polyenoids with n edges with symmetry class C_s.
 * https://oeis.org/A000908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (T(n) - 3*u(n/2) - 3*u((n+1)/2) + 3*u((n-3)/6) -
 *            u(n/3))/6 for n > 0, where T(n) = 3*binomial(2n+3,n)/(2n+3) and
 *            u(m) = binomial(2m,m)/(m+1) if m is an integer, else 0. This
 *            equals OEIS's form a(n) = A003446(n+1) - u((n-3)/6) - (u(n/3) -
 *            u((n-3)/6))/2 - (u(n/2) + (u((n+1)/2) - u((n-3)/6)))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   z908_add, z908_cat, z908_div, z908_mul, z908_set1, z908_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/z908_add.h"
#include "../utils/z908_cat.h"
#include "../utils/z908_div.h"
#include "../utils/z908_mul.h"
#include "../utils/z908_set1.h"
#include "../utils/z908_sub.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t L = (size_t)((2 * n + 3) / 32 + 4);
    uint32_t *acc = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t i;

    /* T = 3*binomial(2n+3,n)/(2n+3): ordered triples of planted Catalan branches */
    z908_set1(acc, L);
    for (i = 1; i <= n; i++) {
        z908_mul(acc, L, (uint32_t)(n + 3 + i));
        z908_div(acc, L, (uint32_t)i);
    }
    z908_mul(acc, L, 3u);
    z908_div(acc, L, (uint32_t)(2 * n + 3));

    /* + 3*s */
    if (n % 6 == 3) {
        z908_cat(tmp, L, (n - 3) / 6);
        z908_mul(tmp, L, 3u);
        z908_add(acc, tmp, L);
    }
    /* - 3*r */
    if (n % 2 == 0) z908_cat(tmp, L, n / 2);
    else z908_cat(tmp, L, (n + 1) / 2);
    z908_mul(tmp, L, 3u);
    z908_sub(acc, tmp, L);
    /* - c */
    if (n % 3 == 0) {
        z908_cat(tmp, L, n / 3);
        z908_sub(acc, tmp, L);
    }

    z908_div(acc, L, 6u);

    size_t top = L;
    while (top > 0 && acc[top - 1] == 0) top--;
    seq_t res = 0;
    while (top > 0) {
        top--;
        int k;
        for (k = 0; k < 4; k++) res = (seq_t)(res * 256u);
        res += (seq_t)acc[top];
    }
    free(acc);
    free(tmp);
    return res;
}
