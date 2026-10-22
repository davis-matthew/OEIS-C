/*
 * A004847  Numbers that are the sum of at most 7 positive 5th powers.
 * https://oeis.org/A004847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- a number x is a term iff the fewest positive 5th powers
 *            summing to x is at most 7 (DP over x); terms listed in
 *            increasing order, starting with 0
 * Range:     u8 n <= 43, u16 n <= 4736, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms_arr = NULL;
static idx_t terms_len = 0;
static uint64_t sieve_lim = 0;

static void build_upto(uint64_t L) {
    unsigned char *mc = (unsigned char *)malloc((size_t)(L + 1));
    uint64_t pw[128];
    int np = 0;
    uint64_t b, x;
    int k;
    for (b = 1; b * b * b * b * b <= L && np < 128; b++)
        pw[np++] = b * b * b * b * b;
    mc[0] = 0;
    for (x = 1; x <= L; x++) {
        unsigned char best = 8;
        for (k = 0; k < np && pw[k] <= x; k++) {
            unsigned char v = (unsigned char)(mc[x - pw[k]] + 1);
            if (v < best) best = v;
        }
        mc[x] = best;
    }
    free(terms_arr);
    terms_len = 0;
    for (x = 0; x <= L; x++)
        if (mc[x] <= 7) terms_len++;
    terms_arr = (uint64_t *)malloc((size_t)terms_len * sizeof(uint64_t));
    terms_len = 0;
    for (x = 0; x <= L; x++)
        if (mc[x] <= 7) terms_arr[terms_len++] = x;
    free(mc);
    sieve_lim = L;
}

seq_t a(idx_t n) {
    if (sieve_lim == 0) build_upto((uint64_t)1 << 16);
    while (terms_len < n) build_upto(sieve_lim * 4);
    return (seq_t)terms_arr[n - 1];
}
