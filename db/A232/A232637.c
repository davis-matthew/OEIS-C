/*
 * A232637  Odious numbers of order 2: a(n) = A000069(A000069(n)).
 * https://oeis.org/A232637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A000069(A000069(n))
 * Range:     u8 n <= 65, u16 n <= 8193, u32 n <= 8193, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Validated: standard, pass, 80 terms through n = 7967 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x has an odd number of 1‑bits (odious). */
static bool odious_parity(seq_t x)
{
    bool p = false;
    while (x) {
        p ^= (bool)(x & 1);
        x >>= 1;
    }
    return p;
}

/* Return the k‑th odious number (k ≥ 1). */
static seq_t nth_odious(idx_t k)
{
    seq_t val = 0;
    idx_t cnt = 0;
    while (cnt < k) {
        ++val;
        if (odious_parity(val))
            ++cnt;
    }
    return val;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    seq_t inter = nth_odious(n);          /* A000069(n)          */
    return nth_odious((idx_t)inter);      /* A000069(A000069(n)) */
}
