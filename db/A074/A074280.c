/*
 * A074280  Duplicate of A000523.
 * https://oeis.org/A074280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ⌊log₂ n⌋ (with a(1)=0)
 * Range:     i8 n <= 36, i16 n <= 36, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 (though not used by the sequence), return 0 */
    if (n == 0) return 0;

    /* Use an efficient bit‑count method.
       If intrinsic clzll is available, use it; otherwise fall back to a simple loop. */
#if defined(__GNUC__) || defined(__clang__)
    /* __builtin_clzll returns the number of leading zero bits in a 64‑bit value.
       For n>0, floor(log2(n)) = 63 - clz(n). */
    return (seq_t)(63 - __builtin_clzll(n));
#else
    idx_t x = n;
    seq_t r = 0;
    while (x >>= 1) ++r;
    return r;
#endif
}
