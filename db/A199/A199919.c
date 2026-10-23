/*
 * A199919  Number of distinct sums of distinct divisors of n when positive
 *          and negative divisors are allowed.
 * https://oeis.org/A199919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of distinct sums obtainable by choosing for each
 *            divisor d|n an exponent e in {-1,0,1} and summing e*d. The
 *            algorithm enumerates all reachable sums by dynamic programming
 *            on the set of signed divisors
 * Range:     u8 n <= 59, u16 n <= 9239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but 0 is not used */

    /* --- collect all divisors of n ------------------------------------- */
    idx_t divs[128];
    size_t divcnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[divcnt++] = i;
            if (i != n / i) divs[divcnt++] = n / i;
        }
    }

    /* --- compute sigma(n) and array size --------------------------------- */
    uint64_t sigma = 0;
    for (size_t k = 0; k < divcnt; ++k) sigma += (uint64_t)divs[k];
    size_t sz = 2 * sigma + 1;            /* range of possible sums [-σ,σ] */
    size_t offset = (size_t)sigma;        /* index representing sum 0 */

    /* --- dynamic programming arrays ------------------------------------- */
    uint8_t *dp_cur  = (uint8_t *)calloc(sz, 1);   /* current reachable sums */
    uint8_t *dp_next = (uint8_t *)calloc(sz, 1);   /* temporary array      */
    if (!dp_cur || !dp_next) {
        free(dp_cur); free(dp_next);
        return 0;        /* allocation failed */
    }
    dp_cur[offset] = 1;  /* only sum 0 is reachable initially */

    /* --- process each divisor --------------------------------------------- */
    for (size_t idx = 0; idx < divcnt; ++idx) {
        uint64_t d = (uint64_t)divs[idx];
        size_t dsize = (size_t)d;

        memcpy(dp_next, dp_cur, sz);          /* preserve e=0 case */

        for (size_t j = 0; j < sz; ++j) {
            if (!dp_cur[j]) continue;
            if (j + dsize < sz) dp_next[j + dsize] = 1;   /* e = +1  */
            if (j >= dsize)     dp_next[j - dsize] = 1;   /* e = -1  */
        }

        /* swap current and next arrays for next iteration */
        uint8_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* --- count distinct sums --------------------------------------------- */
    seq_t result = 0;
    for (size_t j = 0; j < sz; ++j) {
        if (dp_cur[j]) ++result;
    }

    free(dp_cur);
    free(dp_next);
    return result;
}
