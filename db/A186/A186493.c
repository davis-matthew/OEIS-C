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
 * Helpers:   count_pent_less_than_5n, pent_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_pent_less_than_5n.h"
#include "../utils/pent_value.h"

seq_t a(idx_t n) {
    /* 5·n as seq_t. */
    seq_t five_n = (seq_t)5 * (seq_t)n;

    /* Count pentagonal numbers strictly less than 5·n. */
    idx_t pent_cnt = count_pent_less_than_5n(n, five_n);

    /* The rank is n (for the n‑th multiple of 5) plus the count of
       pentagonal numbers smaller than that multiple. */
    return (seq_t)n + (seq_t)pent_cnt;
}
