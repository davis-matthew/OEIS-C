/*
 * A165513  Trapezoidal numbers.
 * https://oeis.org/A165513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th positive integer that can be expressed as a sum
 *            of consecutive integers with the smallest term at least 2.
 *            Computed by brute‑force search
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9897 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_trapezoidal(seq_t x) {
    if (x <= 4) return false;          /* 5 is the first qualifying value */
    for (uint64_t k = 2;; ++k) {
        seq_t kk = (seq_t)k;
        seq_t s  = kk * (kk - 1) / 2;   /* sum of 1..k-1 */
        if (s >= x) break;             /* k too large */
        seq_t rem = x - s;
        if (rem % kk == 0) {
            seq_t a = rem / kk;
            if (a >= 2) return true;   /* representation found */
        }
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;   /* how many trapezoidal numbers found so far */
    seq_t value = 0;   /* current integer being tested */

    while (count < n) {
        ++value;
        if (is_trapezoidal(value))
            ++count;
    }
    return value;
}
