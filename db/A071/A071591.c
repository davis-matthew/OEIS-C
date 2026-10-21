/*
 * A071591  Numbers m such that Reversal(m) is squarefree.
 * https://oeis.org/A071591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Goal: return the n-th positive integer m such that the decimal
 * reversal of m (rev(m)) is square‑free.  Zero will never be returned
 * for n ≥ 1. */
#include <stdbool.h>
#include <stdint.h>

/* Reverse the decimal representation of a 64‑bit integer. */
static inline uint64_t rev_uint64(uint64_t x)
{
    uint64_t r = 0;
    while (x != 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

/* Test whether a 64‑bit integer is square‑free: no prime square divides it. */
static inline bool is_squarefree_uint64(uint64_t x)
{
    uint64_t p = 2;
    while (p * p <= x) {
        if (x % (p * p) == 0)
            return false;
        ++p;
    }
    return true;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                /* n ≥ 1 in normal use; return 0 as a safe default */
        return (seq_t)0;

    uint64_t m = 1;            /* candidate integer to test */
    idx_t count = 0;           /* how many suitable integers seen so far */

    while (true) {
        uint64_t rev = rev_uint64(m);
        if (is_squarefree_uint64(rev)) {
            ++count;
            if (count == n)
                return (seq_t)m;  /* truncate to seq_t; fine for values that do fit */
        }
        ++m;
    }
}
