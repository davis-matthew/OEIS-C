/*
 * A164279  Triangle of 2^n terms per row, a Petoukhov sequence generated from
 *          (3,2).
 * https://oeis.org/A164279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = * let r = ⌊log2(n+1)⌋, p = n - (2^r - 1); * then a(n) =
 *            T(r, p) where * T(0,0) = 1; * T(n,m) = 3 * T(n-1,m) if m <
 *            2^(n-1), * T(n,m) = 2 * T(n-1, 2^n - m - 1) otherwise
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper computing the triangle entry T(n,m). */
static seq_t
_T(idx_t n, idx_t m)
{
    if (n == 0)
        return (seq_t)1;

    /* n > 0, so (1ULL << (n-1)) is safe for the intended limits. */
    idx_t half = ((idx_t)1) << (n - 1);
    if (m < half)
        return ((seq_t)3) * _T(n - 1, m);
    else {
        idx_t mirror = ((idx_t)1 << n) - m - 1;
        return ((seq_t)2) * _T(n - 1, mirror);
    }
}

/* Public function required by the build system. */
seq_t
a(idx_t n)
{
    /* Find row r such that n is in [2^r-1, 2^(r+1)-2]. */
    idx_t r = 0;
    /* Guard against shifting 64 bits which is undefined on C/C++ */
    while (r < 63 && (((idx_t)1) << (r + 1)) <= n + 1)
        r++;

    /* Linear index to triangle coordinates. */
    idx_t start = ((idx_t)1 << r) - 1;
    idx_t pos   = n - start;

    return _T(r, pos);
}
