/*
 * A178665  Expansion of the polynomial
 *          (1+x^3)*(1+x^15)*(1+x^23)*(1+x^27)*(1+x^35)*(1+x^39)*(1+x^47)*(1+x^59).
 * https://oeis.org/A178665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x^3)(1+x^15)(1+x^23)(1+x^27)(1+x^35)(1+x^39)(1+x^47)(1+x^59)
 * Range:     u8 n <= 143, u16 n <= 143, u32 n <= 143, u64 n <= 143,
 *            u128 n <= 143, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const idx_t exponents[8] = {3,15,23,27,35,39,47,59};
static seq_t cache[249] = {0};
static int prepared = 0;

static void prepare_cache(void) {
    if (prepared) return;
    cache[0] = 1;
    for (int i = 0; i < 8; ++i) {
        idx_t e = exponents[i];
        for (idx_t sum = 248; sum >= e; --sum) {
            cache[sum] += cache[sum - e];
        }
    }
    prepared = 1;
}

seq_t a(idx_t n) {
    prepare_cache();
    if (n <= 248) return cache[n];
    return 0;
}
