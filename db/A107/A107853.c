/*
 * A107853  Expansion of g.f. x*(x-1)*(x+1)^3/((2*x^3+x^2-1)*(x^4+1)).
 * https://oeis.org/A107853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   The generating function is G(x) = x*(x-1)*(x+1)^3 /
 *            ((2*x^3+x^2-1)*(x^4+1)) which gives the linear recurrence a(n) =
 *            a(n‑2) + 2*a(n‑3) – a(n‑4) + a(n‑6) + 2*a(n‑7) – p(n) with
 *            initialization a(0)=0, a(1)=1, a(2)=2, a(3)=1, a(4)=2, a(5)=3,
 *            a(6)=2 and p(n) = {0, -1, -2, 0, 2, 1, 0, 0} for n=0…7 (0...
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[7] = {0, 1, 2, 1, 2, 3, 2};
static const int  p_coeff[8]   = {0, -1, -2, 0, 2, 1, 0, 0};

#define MAX_N 20000                    /* sufficient for  n up to 10 000 */
static seq_t cache[MAX_N];
static idx_t  cached_up_to = 0U;        /* indices 0 … cached_up_to-1 are valid */

/* ensure that cache[0..6] are set and that cached_up_to = 7 */
static void init_cache(void) {
    if (cached_up_to > 0) return;
    for (idx_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
    cached_up_to = 7;
}

seq_t a(idx_t n) {
    init_cache();

    /* if n is already computed, return it */
    if (n < cached_up_to) return cache[n];

    /* grow cache up to n (must stay below MAX_N) */
    if (n >= MAX_N) {
        /* failed: target index out of preallocated range */
        return (seq_t)0;
    }

    for (idx_t i = cached_up_to; i <= n; ++i) {
        /* recurrence requires indices ≥ 0; for small i we simply copy base values */
        if (i < 7) {
            /* base already set in init_cache() */
            continue;
        }
        seq_t term = cache[i-2] + (seq_t)2 * cache[i-3];
        term -= cache[i-4];
        term += cache[i-6];
        term += (seq_t)2 * cache[i-7];
        if (i <= 7) term -= (seq_t)p_coeff[i];
        cache[i] = term;
    }
    cached_up_to = n + 1;
    return cache[n];
}
