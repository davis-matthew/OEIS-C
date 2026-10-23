/*
 * A118180  Triangle T(n, k) = 3^(k*(n-k)), read by rows.
 * https://oeis.org/A118180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3^(k*(n-k)) where (row, column) is found by writing the
 *            triangle T(n,k)=3^(k*(n-k)) row‑by‑row. The sequence is the
 *            flattened triangle (offset 0)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 57, u64 n <= 96, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t triangular_count(idx_t r) { return (r + 1) * (r + 2) / 2; }

/* fast modular exponentiation of 3 for an integer exponent */
static inline seq_t pow3_uint64(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 3;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Find the row r such that triangular_count(r) > n */
    idx_t r = 0;
    while (triangular_count(r) <= n) ++r;
    /* cumulative count up to previous row */
    idx_t prev = r * (r + 1) / 2;
    /* column index within the row */
    idx_t k = n - prev;
    /* exponent k*(r-k) */
    idx_t exp = k * (r - k);
    return pow3_uint64(exp);
}
