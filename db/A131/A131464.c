/*
 * A131464  a(n) = 4*n^3 - 3*n^2 + 2*n - 1.
 * https://oeis.org/A131464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 - 3*n^2 + 2*n - 1
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 (outside the OEIS offset) return 0.  */
    if (n == 0)
        return (seq_t)0;

#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit arithmetic to avoid overflow on large n.  */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 res = 4*nn*nn*nn - 3*nn*nn + 2*nn - 1;
    return (seq_t)res;
#else
    /* Fallback: 64‑bit intermediate.  Works for all n that fit in
       the public test ranges (up to about 10 000 for 64‑bit builds).  */
    uint64_t nn = (uint64_t)n;
    uint64_t res = 4*nn*nn*nn - 3*nn*nn + 2*nn - 1;
    return (seq_t)res;
#endif
}
