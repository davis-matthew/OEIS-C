/*
 * A004793  a(1)=1, a(2)=3; a(n) is least k such that no three terms of a(1),
 *          a(2), ..., a(n-1), k form an arithmetic progression.
 * https://oeis.org/A004793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n-1), where b(0)=1, b(2m)=3b(m)-2-3[m odd],
 *            b(2m+1)=3b(m)-3[m odd]
 * Range:     u8 n <= 38, u16 n <= 1280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_b.h"

seq_t a(idx_t n) {
    return compute_b(n - 1);
}
