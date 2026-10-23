/*
 * A023867  a(n) = 1*t(n) + 2*t(n-1) + ...+ k*t(n+1-k), where k=floor((n+1)/2)
 *          and t is A001950 (upper Wythoff sequence).
 * https://oeis.org/A023867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} i * t(n+1-i), where t(m) = ⌊m*φ²⌋
 *            (A001950, the upper Wythoff sequence)
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* compute upper Wythoff sequence term t(m) = ⌊m * φ²⌋ */
static inline seq_t t_value(idx_t m)
{
    seq_t mm = (seq_t)m;
    seq_t n2 = mm * mm;                 /* m² */
    seq_t five_n2 = n2 * (seq_t)5;      /* 5·m² */
    seq_t s5n = floor_sqrt(five_n2);    /* ⌊m·√5⌋ = ⌊√(5·m²)⌋ */
    seq_t fphi = (mm + s5n) / (seq_t)2; /* ⌊m·φ⌋ = ⌊(m + ⌊m√5⌋)/2⌋ */
    return mm + fphi;                   /* ⌊m·φ²⌋ = m + ⌊m·φ⌋ */
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;    /* offset is 1 – value below is irrelevant */

    idx_t k = (n + 1) / 2;   /* ⌊(n+1)/2⌋ */
    seq_t sum = (seq_t)0;

    for (idx_t i = 1; i <= k; ++i) {
        idx_t t_index = n + 1 - i;      /* argument of t() */
        seq_t term = t_value(t_index);  /* t(t_index) */
        sum += (seq_t)i * term;
    }

    return sum;
}
