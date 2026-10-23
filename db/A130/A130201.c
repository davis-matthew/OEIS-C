/*
 * A130201  Odious oblong (promic) numbers.
 * https://oeis.org/A130201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * A130202(n) A130202(n) is the nth odious triangular
 *            number, i.e. the nth k such that T_k = k*(k+1)/2 has an odd
 *            number of 1‑bits in binary
 * Range:     u8 n <= 5, u16 n <= 117, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Returns true iff x has an odd number of 1 bits (odious). */
static bool is_odious(seq_t x) {
    seq_t parity = 0;
    while (x > 0) {
        parity ^= (x & 1);
        x >>= 1;
    }
    return parity != 0;
}

/* a(n) returns the nth term of OEIS A130201. */
seq_t a(idx_t n) {
    idx_t remaining = n;          /* count of odious triangular numbers still needed */
    for (idx_t k = 1; ; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t t = kseq * (kseq + 1) / 2;   /* triangular number T_k */
        if (is_odious(t)) {
            --remaining;
            if (remaining == 0) {
                return t << 1;          /* 2 * T_k */
            }
        }
    }
}
