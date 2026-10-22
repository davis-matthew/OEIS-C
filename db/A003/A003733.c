/*
 * A003733  Number of spanning trees in C_5 X P_n.
 * https://oeis.org/A003733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: 5*x*(1+x)*(1+41*x-998*x^2+2722*x^3-998*x^4+41*x^5+x^6) /
 *            ((1-x)*(1-279*x+961*x^2-279*x^3+x^4)*(1-39*x+281*x^2-39*x^3+x^4)),
 *            i.e. a(n) = 319*a(n-1) - 12441*a(n-2) + 128319*a(n-3) -
 *            408001*a(n-4) + 408001*a(n-5) - 128319*a(n-6) + 12441*a(n-7) -
 *            319*a(n-8) + a(n-9) + c(n), where c(n) is the co...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t computed = 0; /* cache[0..computed] valid */

static void ensure_cap(idx_t need) {
    idx_t i, newcap;
    seq_t *nc;
    if (need < cache_cap) return;
    newcap = cache_cap ? cache_cap : 16;
    while (newcap <= need) newcap *= 2;
    nc = (seq_t *)malloc(newcap * sizeof(seq_t));
    for (i = 0; i < newcap; i++) nc[i] = 0;
    for (i = 0; i < cache_cap; i++) nc[i] = cache[i];
    free(cache);
    cache = nc;
    cache_cap = newcap;
}

/* a(i) for i possibly <= 0 (given as i + 9 offset to stay unsigned) */
static seq_t get(idx_t i_plus9) {
    if (i_plus9 <= 9) return 0;
    return cache[i_plus9 - 9];
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (cache == NULL) {
        ensure_cap(16);
        cache[0] = 0;
        computed = 0;
    }
    if (n <= computed) return cache[n];

    ensure_cap(n);
    while (computed < n) {
        idx_t i = computed + 1;
        idx_t k = i + 9;
        seq_t pos = get(k - 1) * 319
                  + get(k - 3) * 128319
                  + get(k - 5) * 408001
                  + get(k - 7) * 12441
                  + get(k - 9);
        seq_t neg = get(k - 2) * 12441
                  + get(k - 4) * 408001
                  + get(k - 6) * 128319
                  + get(k - 8) * 319;
        /* numerator 5*x*(1+x)*P(x) contributions */
        switch (i) {
            case 1: pos += 5; break;
            case 2: pos += 210; break;
            case 3: neg += 4785; break;
            case 4: pos += 8620; break;
            case 5: pos += 8620; break;
            case 6: neg += 4785; break;
            case 7: pos += 210; break;
            case 8: pos += 5; break;
            default: break;
        }
        cache[i] = pos - neg;
        computed = i;
    }
    return cache[n];
}
