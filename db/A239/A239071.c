/*
 * A239071  Numbers k such that k+x+y is a triangular number (A000217), where
 *          x and y are the two triangular numbers nearest to k.
 * https://oeis.org/A239071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A239071: a(n) = the nth integer k such that k + x + y is triangular,
 *   where x and y are the nearest triangular numbers to k.
 *   (x is the largest triangular ≤ k, y is the smallest triangular ≥ k;
 *   for k triangular, y = k and x = the previous triangular, or 0 if k=0).
 *
 * The function computes terms on demand by brute‑force search.
 * It uses only integer arithmetic; no floating point or math library.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the integer bit length of a value (number of bits needed to represent it). */
static inline size_t bitlen_seq_t(seq_t v) {
    size_t bits = 0;
    while (v) {
        v >>= 1;
        ++bits;
    }
    return bits;
}

/* Integer square root: returns ⌊√v⌋ for non‑negative v. */
static inline seq_t isqrt_seq(seq_t v) {
    if (v == 0) return 0;
    size_t bits = bitlen_seq_t(v);
    seq_t low  = 0;
    seq_t high = ((seq_t)1) << ((bits + 1) / 2);   /* safe upper bound */
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        if (mid <= v / mid) {          /* mid^2 ≤ v, safely computed */
            seq_t sq = mid * mid;
            if (sq == v) return mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Test whether v is triangular: 8*v+1 must be a perfect square. */
static inline bool is_triangular_seq(seq_t v) {
    seq_t d = 8 * v + 1;
    seq_t r = isqrt_seq(d);
    return r * r == d;
}

/* Main sequence entry point. */
seq_t a(idx_t n) {
    idx_t counted = 0;
    seq_t k = 0;
    while (1) {
        /* Find m with T_m ≤ k < T_{m+1} using the same sqrt trick. */
        seq_t d      = 8 * k + 1;
        seq_t sqrt_d = isqrt_seq(d);
        idx_t m      = (idx_t)((sqrt_d - 1) / 2);

        seq_t Tm = (seq_t)m * (m + 1) / 2;
        seq_t x, y;

        if (k == Tm) {           /* k itself is triangular */
            seq_t Tprev = (m == 0) ? (seq_t)0 : (seq_t)(m - 1) * m / 2;
            x = Tprev;
            y = k;
        } else {                  /* in between two triangular numbers */
            seq_t Tnext = (seq_t)(m + 1) * (m + 2) / 2;
            x = Tm;
            y = Tnext;
        }

        seq_t sum = k + x + y;
        if (is_triangular_seq(sum)) {
            ++counted;
            if (counted == n) return k;
        }
        ++k;
    }
}
