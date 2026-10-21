/*
 * A098439  Expansion of 1/sqrt(1-2x-47x^2).
 * https://oeis.org/A098439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(n) = ((2*n-1)*a(n-1) + 47*(n-1)*a(n-2)) / n (n
 *            ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses 128‑bit arithmetic for the 64‑bit
    builtin build (overflow would otherwise occur).  When seq_t
    is an arbitrary‑precision or 128‑bit type we rely on the
    library's own arithmetic, which is exact.  The code is
    valid both as C11 and C++17. */

#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
#ifdef __cplusplus
        /* C++: detect if seq_t is the 64‑bit builtin type */
        if constexpr (std::is_same<seq_t, unsigned long long>::value ||
                      std::is_same<seq_t, uint64_t>::value) {
            unsigned __int128 t1 = (unsigned __int128)(2*i-1) * (unsigned __int128)a1;
            unsigned __int128 t2 = (unsigned __int128)47 * (unsigned __int128)(i-1) *
                                   (unsigned __int128)a0;
            an = (seq_t)((t1 + t2) / i);
        } else {
            /* arbitrary‑precision or 128‑bit type: use the
               library's arithmetic (no 128‑bit cast needed). */
            an = (((seq_t)(2*i-1) * a1) + (seq_t)47*(i-1) * a0) / i;
        }
#else
        /* C: seq_t is a builtin integer type, use 128‑bit intermediates. */
        unsigned __int128 t1 = (unsigned __int128)(2*i-1) * (unsigned __int128)a1;
        unsigned __int128 t2 = (unsigned __int128)47 * (unsigned __int128)(i-1) *
                               (unsigned __int128)a0;
        an = (seq_t)((t1 + t2) / i);
#endif
        a0 = a1;
        a1 = an;
    }

    return a1;
}
