/*
 * A053628  Smallest integer which is the harmonic mean of A005279(n) and an
 *          integer.
 * https://oeis.org/A053628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over positive integers x of 2*m*x/(m+x) (an integer),
 *            where m = A005279(n); search over x = 1..m
 * Range:     u8 n <= 5, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t A053628_collect_divisors(seq_t m, seq_t *divs, idx_t max_divs) {
    idx_t count = 0;
    for (seq_t d = 1; d * d <= m && count < max_divs; d++) {
        if (m % d == 0) {
            divs[count++] = d;
            if (d != m / d && count < max_divs) {
                divs[count++] = m / d;
            }
        }
    }
    return count;
}

static bool A053628_has_divisor_pair(seq_t m) {
    seq_t divs[512];
    idx_t count = A053628_collect_divisors(m, divs, 512);
    for (idx_t i = 0; i < count; i++) {
        for (idx_t j = 0; j < count; j++) {
            seq_t d = divs[i];
            seq_t e = divs[j];
            if (d < e && e < 2 * d) {
                return true;
            }
        }
    }
    return false;
}

static seq_t A053628_A005279_a(idx_t n) {
    idx_t count = 0;
    seq_t m = 1;
    while (count < n) {
        m++;
        if (A053628_has_divisor_pair(m)) {
            count++;
        }
    }
    return m;
}

seq_t a(idx_t n) {
    seq_t m = A053628_A005279_a(n);
    /* HM(m,x) is increasing in x for x <= m, so the first valid x gives the minimum */
    for (seq_t x = 1; x <= m; x++) {
        seq_t num = 2 * m * x;
        seq_t den = m + x;
        if (num % den == 0) {
            return num / den;
        }
    }
    return m;
}
