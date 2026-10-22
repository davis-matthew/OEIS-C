/*
 * A007550  Natural numbers exponentiated twice.
 * https://oeis.org/A007550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} k^(n-k) * binomial(n,k) * Bell(k), with 0^0
 *            = 1; Bell numbers via the Bell triangle
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   a7550_pow, alloc_zero, binom, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/binom.h"
#include "../utils/a7550_pow.h"

static seq_t *a7550_bells(idx_t n) {
    seq_t *bells = alloc_zero(n + 1);
    seq_t *prev = alloc_zero(n + 1);
    seq_t *curr = alloc_zero(n + 1);
    seq_t *tmp;
    idx_t i, j;

    bells[0] = 1;
    prev[0] = 1;
    for (i = 1; i <= n; i++) {
        curr[0] = prev[i - 1];
        for (j = 1; j <= i; j++) {
            curr[j] = curr[j - 1] + prev[j - 1];
        }
        bells[i] = curr[0];
        tmp = prev;
        prev = curr;
        curr = tmp;
    }

    release(prev, n + 1);
    release(curr, n + 1);
    return bells;
}

seq_t a(idx_t n) {
    seq_t *bells = a7550_bells(n);
    seq_t result = 0;
    idx_t k;

    for (k = 0; k <= n; k++) {
        seq_t power = a7550_pow((seq_t)k, n - k);
        seq_t term = power * binom(n, k);
        term *= bells[k];
        result += term;
    }

    release(bells, n + 1);
    return result;
}
