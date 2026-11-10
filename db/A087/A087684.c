/*
 * A087684  Consecutive repeating elements in sequence A006158.
 * https://oeis.org/A087684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th distinct value v of A006158 that occurs
 *            consecutively at least twice (A006158(i) = A006158(i+1));
 *            A006158(n) = A006158(A006158(n-3)) + A006158(n - A006158(n-3)),
 *            A006158(1..3) = 1
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A087684_b = NULL;
static idx_t A087684_b_len = 0;
static idx_t A087684_b_cap = 0;
static uint64_t *A087684_r = NULL;
static idx_t A087684_r_cnt = 0;
static idx_t A087684_r_cap = 0;

static int A087684_step(void) {
    if (A087684_b_len >= A087684_b_cap) {
        idx_t nc = A087684_b_cap < 256 ? 256 : A087684_b_cap * 2;
        uint64_t *p = (uint64_t *)realloc(A087684_b, (size_t)nc * sizeof(uint64_t));
        if (!p) return 0;
        A087684_b = p;
        A087684_b_cap = nc;
    }
    idx_t m = A087684_b_len + 1;
    uint64_t v;
    if (m <= 3) {
        v = 1;
    } else {
        uint64_t t = A087684_b[m - 4];
        v = A087684_b[t - 1] + A087684_b[m - t - 1];
    }
    A087684_b[m - 1] = v;
    A087684_b_len = m;
    if (m >= 2 && A087684_b[m - 2] == v &&
        (A087684_r_cnt == 0 || A087684_r[A087684_r_cnt - 1] != v)) {
        if (A087684_r_cnt >= A087684_r_cap) {
            idx_t nc = A087684_r_cap < 64 ? 64 : A087684_r_cap * 2;
            uint64_t *p = (uint64_t *)realloc(A087684_r, (size_t)nc * sizeof(uint64_t));
            if (!p) return 0;
            A087684_r = p;
            A087684_r_cap = nc;
        }
        A087684_r[A087684_r_cnt++] = v;
    }
    return 1;
}

seq_t a(idx_t n) {
    while (A087684_r_cnt < n) {
        if (!A087684_step()) return (seq_t)0;
    }
    return (seq_t)A087684_r[n - 1];
}
