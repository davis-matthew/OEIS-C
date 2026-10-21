/*
 * A089215  Thue-Morse sequence on the integers.
 * https://oeis.org/A089215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2. For n>2, let k be the smallest integer such that
 *            2^k >= n. If n <= 2^{k-1}, a(n) = a(n−2^{k-1}); otherwise a(n) =
 *            (k+2) – a(n−2^{k-1}); This follows directly from the Thue–Morse
 *            construction with M(k‑1)=k+2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

static seq_t _a_rec(idx_t level, idx_t n)
{
    /* level == 1 corresponds to the base sequence {1,2} */
    if (level == 1) {
        return (n == 1) ? (seq_t)1 : (seq_t)2;
    }

    idx_t half = 1ULL << (level - 1);   /* 2^{level-1} */

    if (n <= half) {
        return _a_rec(level - 1, n);
    } else {
        /* (level+2) is the value M(level-1) */
        return (seq_t)(level + 2) - _a_rec(level - 1, n - half);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    idx_t level = 1;
    idx_t len   = 2;   /* 2^level */

    while (len < n) {
        /* avoid shifting beyond 2^63 to prevent overflow of 64‑bit idx_t */
        if (len > UINT64_MAX >> 1) {
            level = 64;
            break;
        }
        len <<= 1;
        level++;
    }

    return _a_rec(level, n);
}
