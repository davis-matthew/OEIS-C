/*
 * A033720  Product theta3(q^d); d | 8.
 * https://oeis.org/A033720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of integer solutions to a^2 + 2*b^2 + 4*c^2 + 8*d^2 = n
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Return ⌊√n⌋ for unsigned 64‑bit integers, no overflow. */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1))
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Allocate arrays for partial sums of (a,b) and (c,d). */
    size_t sz = (size_t)(n + 1);
    seq_t *AB = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *CD = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!AB || !CD) {
        /* Allocation failure – unlikely for small n. */
        free(AB);
        free(CD);
        return (seq_t)0;
    }

    /* Count contributions from a^2 + 2*b^2. */
    idx_t max_a = floor_sqrt_u64(n);
    for (idx_t a_val = 0; a_val <= max_a; ++a_val) {
        idx_t val_a = a_val * a_val;
        seq_t mult_a = (a_val == 0) ? (seq_t)1 : (seq_t)2;
        idx_t max_b = floor_sqrt_u64(n / 2);
        for (idx_t b_val = 0; b_val <= max_b; ++b_val) {
            idx_t val_b = 2 * b_val * b_val;
            if (val_a + val_b > n)
                break;
            seq_t mult_b = (b_val == 0) ? (seq_t)1 : (seq_t)2;
            AB[(size_t)(val_a + val_b)] += mult_a * mult_b;
        }
    }

    /* Count contributions from 4*c^2 + 8*d^2. */
    idx_t max_c = floor_sqrt_u64(n / 4);
    idx_t max_d = floor_sqrt_u64(n / 8);
    for (idx_t c_val = 0; c_val <= max_c; ++c_val) {
        idx_t val_c = 4 * c_val * c_val;
        seq_t mult_c = (c_val == 0) ? (seq_t)1 : (seq_t)2;
        for (idx_t d_val = 0; d_val <= max_d; ++d_val) {
            idx_t val_d = 8 * d_val * d_val;
            if (val_c + val_d > n)
                break;
            seq_t mult_d = (d_val == 0) ? (seq_t)1 : (seq_t)2;
            CD[(size_t)(val_c + val_d)] += mult_c * mult_d;
        }
    }

    /* Combine partial sums to get total representations. */
    seq_t result = (seq_t)0;
    for (idx_t s = 0; s <= n; ++s)
        result += AB[(size_t)s] * CD[(size_t)(n - s)];

    free(AB);
    free(CD);
    return result;
}
