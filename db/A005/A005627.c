/*
 * A005627  Number of achiral planted trees with n nodes.
 * https://oeis.org/A005627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n+1) = Sum_{k=0..floor(n/2)} s(k)*a(n-2*k), where
 *            s(n) = A000625(n): s(0) = 1, s(n+1) = ([x^n] s(x)^3 + 2*[x^n]
 *            s(x^3)) / 3, i.e. S(x) = 1 + x*(S(x)^3 + 2*S(x^3))/3
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *A5627_s = NULL, *A5627_sq = NULL, *A5627_cu = NULL;
static idx_t A5627_scap = 0, A5627_ns = 0;

static seq_t *A5627_a = NULL;
static idx_t A5627_acap = 0, A5627_na = 0;

static seq_t A5627_div3(seq_t t) {
    if (seq_is_fixed_width()) {
        /* exact division by 3 modulo 2^w: multiply by the inverse of 3 */
        seq_t x = 3;
        int it;
        for (it = 0; it < 7; it++) {
            seq_t u = (seq_t)3 * x;
            u = (seq_t)2 - u;
            x = x * u;
        }
        return t * x;
    }
    return t / 3;
}

static void A5627_ensure_s(idx_t m) {
    if (m >= A5627_scap) {
        idx_t nc = (m / 256 + 1) * 256;
        A5627_s = grow_zero(A5627_s, A5627_scap, nc);
        A5627_sq = grow_zero(A5627_sq, A5627_scap, nc);
        A5627_cu = grow_zero(A5627_cu, A5627_scap, nc);
        A5627_scap = nc;
    }
    while (A5627_ns <= m) {
        idx_t i = A5627_ns, k;
        if (i == 0) {
            A5627_s[0] = 1;
            A5627_sq[0] = 1;
            A5627_cu[0] = 1;
        } else {
            idx_t j = i - 1;
            seq_t t = A5627_cu[j];
            seq_t sq = 0, cu = 0;
            if (j % 3 == 0) t += (seq_t)2 * A5627_s[j / 3];
            A5627_s[i] = A5627_div3(t);
            for (k = 0; k <= i; k++) sq += A5627_s[k] * A5627_s[i - k];
            A5627_sq[i] = sq;
            for (k = 0; k <= i; k++) cu += A5627_s[k] * A5627_sq[i - k];
            A5627_cu[i] = cu;
        }
        A5627_ns++;
    }
}

seq_t a(idx_t n) {
    if (n >= A5627_acap) {
        idx_t nc = (n / 256 + 1) * 256;
        A5627_a = grow_zero(A5627_a, A5627_acap, nc);
        A5627_acap = nc;
    }
    while (A5627_na <= n) {
        idx_t i = A5627_na;
        if (i == 0) {
            A5627_a[0] = 1;
        } else {
            idx_t m = i - 1, k;
            seq_t sum = 0;
            A5627_ensure_s(m / 2);
            for (k = 0; k <= m / 2; k++) sum += A5627_s[k] * A5627_a[m - 2 * k];
            A5627_a[i] = sum;
        }
        A5627_na++;
    }
    return A5627_a[n];
}
