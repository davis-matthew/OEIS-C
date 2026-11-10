/*
 * A057928  Position at which 2^n occurs in A057926, or -1 if it does not
 *          occur.
 * https://oeis.org/A057928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = position of 2^n in A057926 (a(0)=1, a(1)=3, a(k) = least
 *            y>0 with (a(k-1) OR y) > (a(k-2) OR a(k-1))), or -1 if 2^n never
 *            occurs. Computed by direct generation of A057926 with a
 *            closed-form O(64) next-term step
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A057928_init = false;
static uint64_t A057928_p, A057928_B;     /* last term, and OR of last two terms */
static uint64_t A057928_idx;              /* index of last term */
static bool A057928_found[64];
static uint64_t A057928_pos[64];

static void A057928_record(uint64_t v, uint64_t i) {
    if (v != 0 && (v & (v - 1)) == 0) {
        int k = 0;
        while (((v >> k) & 1) == 0) ++k;
        if (!A057928_found[k]) {
            A057928_found[k] = true;
            A057928_pos[k] = i;
        }
    }
}

static uint64_t A057928_next(uint64_t p, uint64_t B) {
    uint64_t best = ~(uint64_t)0;
    for (int k = 0; k < 63; ++k) {
        if ((B >> k) & 1) continue;
        uint64_t hi = ~(uint64_t)0 << (k + 1);
        if ((p & hi) & ~B) continue;
        uint64_t y = (B & ~p) & hi;
        if (!((p >> k) & 1)) y |= (uint64_t)1 << k;
        if (y != 0 && y < best) best = y;
    }
    return best;
}

seq_t a(idx_t n) {
    if (!A057928_init) {
        A057928_init = true;
        for (int i = 0; i < 64; ++i) { A057928_found[i] = false; A057928_pos[i] = 0; }
        A057928_record(1, 0);
        A057928_record(3, 1);
        A057928_p = 3;
        A057928_B = 3;
        A057928_idx = 1;
    }
    if (n >= 62) return (seq_t)-1;
    int e = (int)n;
    while (!A057928_found[e]) {
        if (A057928_p > ((uint64_t)1 << e)) return (seq_t)-1;
        uint64_t y = A057928_next(A057928_p, A057928_B);
        uint64_t nb = A057928_p | y;
        A057928_p = y;
        A057928_B = nb;
        ++A057928_idx;
        A057928_record(y, A057928_idx);
    }
    return (seq_t)A057928_pos[e];
}
