/*
 * A186493  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=5i and g(j)=j-th pentagonal
 *          number. Complement of A186494.
 * https://oeis.org/A186493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + #{ j ≥ 1 : P(j) < 5·n }, where P(j) = j(3j-1)/2 (the
 *            j‑th pentagonal number). The term a(n) is the rank of the number
 *            5·n in the merged sequence of multiples of 5 and pentagonal
 *            numbers, with 5·k listed before P(k) when equal
 * Range:     u8 n <= 51, u16 n <= 115, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Validated: partial, pass, 115 terms through n = 115 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 pent_value(idx_t j) {
    /* Compute P(j) = j(3j-1)/2 safely in 128‑bit arithmetic. */
    unsigned __int128 jj = j;
    return jj * (3*jj - 1) / 2;
}

static inline idx_t count_pent_less_than_5n(idx_t n, seq_t t_val) {
    /* Return the largest j such that P(j) < t_val.  This equals the
       number of pentagonal numbers smaller than t_val. */
    idx_t low = 0;
    idx_t high = 1;
    /* Exponential search to find an upper bound. */
    while (pent_value(high) < (unsigned __int128)t_val) {
        high <<= 1;
    }
    /* Binary search for the last j with P(j) < t_val. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (pent_value(mid) < (unsigned __int128)t_val) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    /* 5·n as seq_t. */
    seq_t five_n = (seq_t)5 * (seq_t)n;

    /* Count pentagonal numbers strictly less than 5·n. */
    idx_t pent_cnt = count_pent_less_than_5n(n, five_n);

    /* The rank is n (for the n‑th multiple of 5) plus the count of
       pentagonal numbers smaller than that multiple. */
    return (seq_t)n + (seq_t)pent_cnt;
}
