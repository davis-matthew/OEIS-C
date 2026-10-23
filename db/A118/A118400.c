/*
 * A118400  Triangle T, read by rows, where all columns of T are different and
 *          yet all columns of the matrix square T^2 (A118401) are equal; a
 *          signed version of triangle A087698.
 * https://oeis.org/A118400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) read row‑by‑row from the triangle T where T(n,k) = (-1)^n *
 *            (C(n,k) – 2*C(n–2,k–1)), with the special case T(1,0) = 1
 * Range:     i8 n <= 59, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are provided by the build. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

/* Compute binomial coefficient C(n,k) as seq_t.
   Uses multiplicative formula with 128‑bit intermediate
   to prevent overflow when seq_t is 64‑bit. */
static seq_t binom_fast(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;  /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
#ifdef __SIZEOF_INT128__
        __int128 prod = (__int128)res * (n - i + 1);
        res = (seq_t)(prod / i);
#else
        /* Fallback – may overflow if seq_t is 64‑bit and product exceeds 63 bits. */
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
#endif
    }
    return res;
}

seq_t a(idx_t n) {
    /* Special case: a(0) and a(1) are simple. */
    if (n == 0) return 1;
    if (n == 1) return 1;          /* because T(1,0) = 1, T(1,1) = -1 is handled below */

    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t s = int_sqrt(8 * n + 1);
    idx_t r = (s - 1) / 2;
    /* Correct possible off‑by‑one due to floor sqrt. */
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Position within the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Special rule: T(1,0) = 1. */
    if (r == 1 && k == 0) return 1;

    seq_t c1 = binom_fast(r, k);
    seq_t c2 = 0;
    if (k > 0 && r >= 2)
        c2 = binom_fast(r - 2, k - 1);

    seq_t diff = c1 - ((seq_t)2 * c2);
    if (r & 1) diff = -diff;   /* apply (-1)^r */
    return diff;
}
