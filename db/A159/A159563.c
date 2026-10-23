/*
 * A159563  Numerator of Hermite(n, 17/18).
 * https://oeis.org/A159563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 17*a(n-1) - 162*(n-1)*a(n-2), a(0)=1, a(1)=17
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 12, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Big‑integer build (C++17).  seq_t is a user‑defined type that
       supports ordinary arithmetic operators. */
    if (n == 0) return 1;
    if (n == 1) return 17;
    seq_t prev2 = 1;          /* a(0) */
    seq_t prev1 = 17;         /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term2 = prev2 * seq_t(162 * (i-1));
        cur = 17 * prev1 - term2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
#else
    /* Integer build (C).  Use 128‑bit intermediate to avoid overflow. */
    if (n == 0) return 1;
    if (n == 1) return 17;
    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 17;  /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        __int128 t1 = (__int128)17 * prev1;
        __int128 t2 = (__int128)162 * (i-1) * prev2;
        __int128 res = t1 - t2;
        cur = (seq_t)res;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
#endif
}
