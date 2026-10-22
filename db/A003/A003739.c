/*
 * A003739  Number of spanning trees in W_5 X P_n.
 * https://oeis.org/A003739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.
 *            45*x*(1+73*x-3456*x^2+4534*x^3+4534*x^4-3456*x^5+73*x^6+x^7)/(1-1152*x+80640*x^2-1442883*x^3+4477824*x^4-4477824*x^5+1442883*x^6-80640*x^7+1152*x^8-x^9),
 *            i.e. a(n) = c(n) + 1152*a(n-1) - 80640*a(n-2) + 1442883*a(n-3) -
 *            4477824*a(n-4) + 4477824*a(n-5) - 1442883*a(n-6) + 80640*a(n-7)
 *            - 1152*a(...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* cache[i] = a(i), cache[0] = 0 */
    static idx_t cap = 0;
    static idx_t done = 0;        /* cache[0..done] valid */
    static const long long numc[9] = {0, 45, 3285, -155520, 204030, 204030, -155520, 3285, 45};
    static const long long den[9] = {1152, -80640, 1442883, -4477824, 4477824, -1442883, 80640, -1152, 1};

    if (n < 1) return 0;

    if (cache == NULL) {
        cap = (n < 64 ? 64 : n + 1);
        cache = grow_zero(NULL, 0, cap);
        cache[0] = 0;
        done = 0;
    }
    if (n >= cap) {
        idx_t ncap = (n + 1 > 2 * cap) ? n + 1 : 2 * cap;
        cache = grow_zero(cache, cap, ncap);
        cap = ncap;
    }

    for (idx_t i = done + 1; i <= n; i++) {
        seq_t pos = 0, neg = 0;
        if (i <= 8) {
            long long c = numc[i];
            if (c >= 0) pos += (seq_t)c;
            else neg += (seq_t)(-c);
        }
        for (idx_t j = 1; j <= 9; j++) {
            if (j > i - 1) break;          /* a(i-j) = 0 for i-j <= 0 */
            long long d = den[j - 1];
            if (d >= 0) pos += (seq_t)d * cache[i - j];
            else neg += (seq_t)(-d) * cache[i - j];
        }
        cache[i] = pos - neg;
        done = i;
    }

    return cache[n];
}
