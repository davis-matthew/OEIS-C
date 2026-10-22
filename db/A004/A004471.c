/*
 * A004471  Nim product 6 * n.
 * https://oeis.org/A004471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(6,n), the nim product 6 (x) n, computed by
 *            recursive splitting on Fermat 2-powers D = 2^(2^k), using D (x)
 *            D = D ^ (D/2)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   nim_mul_w
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/nim_mul_w.h"

seq_t a(idx_t n) {
	int L = 4;
	while (L < 64 && (n >> L) != 0) L *= 2;
	return nim_mul_w((seq_t)6, (seq_t)n, L);
}
