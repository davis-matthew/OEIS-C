/*
 * A062838  Cubes of squarefree numbers.
 * https://oeis.org/A062838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = s_n³ where s_n is the n‑th positive squarefree integer
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 992, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_squarefree(idx_t x) {
    if (x < 4) return true;          /* 1, 2, 3 are squarefree */
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % (p * p) == 0) return false;  /* p² divides x → not squarefree */
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t num   = 0;                  /* candidate squarefree number */

    while (count < n) {
        ++num;
        if (is_squarefree(num)) ++count;
    }

    seq_t v = (seq_t)num;
    return v * v * v;                 /* cube of the n‑th squarefree number */
}
