/*
 * A004441  Numbers that are not the sum of 4 distinct nonzero squares.
 * https://oeis.org/A004441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve by brute-force search for a(1)..a(120) (n not a sum of 4
 *            distinct nonzero squares); for n > 120, a(n) = 4*a(n-35)
 * Range:     u8 n <= 96, u16 n <= 236, u32 n <= 516, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 992 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static bool is_sum_of_4_distinct_squares(unsigned long long m) {
    unsigned long long a, b, c;
    for (a = 1; 4 * a * a < m; a++) {
        unsigned long long a2 = a * a;
        for (b = a + 1; a2 + 3 * b * b < m; b++) {
            unsigned long long b2 = b * b;
            for (c = b + 1; a2 + b2 + 2 * c * c < m; c++) {
                unsigned long long c2 = c * c;
                unsigned long long d2 = m - a2 - b2 - c2;
                unsigned long long d = c + 1;
                while (d * d < d2) d++;
                if (d * d == d2) return true;
            }
        }
    }
    return false;
}

static unsigned long long base_terms[120];
static int base_ready = 0;

static void init_base(void) {
    int count = 0;
    unsigned long long m = 1;
    while (count < 120) {
        if (!is_sum_of_4_distinct_squares(m)) base_terms[count++] = m;
        m++;
    }
    base_ready = 1;
}

seq_t a(idx_t n) {
    const idx_t THRESHOLD = 120;
    const idx_t STEP = 35;
    idx_t k = 0, base_n = n;
    seq_t result;
    idx_t i;

    if (!base_ready) init_base();

    if (n > THRESHOLD) {
        k = (n - THRESHOLD + STEP - 1) / STEP;
        base_n = n - STEP * k;
    }

    result = (seq_t)base_terms[base_n - 1];
    for (i = 0; i < k; i++) result = result * 4;
    return result;
}
