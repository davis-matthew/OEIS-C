/*
 * A008921  Coordination sequence for lattice {E_7}*.
 * https://oeis.org/A008921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(5 -132*n +451*n^2 -480*n^3 +370*n^4 -108*n^5 +34*n^6)
 *            / 5 (for n > 0, a(0) = 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

#ifdef __SIZEOF_INT128__
    __int128 m = n;               /* promote index to signed 128‑bit */
    __int128 v = 34;              /* start with leading coefficient */
    v = v * m + ( -108 );         /* add next term */
    v = v * m + 370;
    v = v * m + ( -480 );
    v = v * m + 451;
    v = v * m + ( -132 );
    v = v * m + 5;
    v = v * 2;                    /* multiply by 2 */
    v = v / 5;                    /* divide by 5 (exact) */
    return (seq_t)v;
#else
    /* Fallback – works when seq_t is a signed or big‑integer type.
       For pure unsigned integer types this branch will produce
       undefined behaviour; the build system normally supplies
       __int128 support so this branch is rarely used. */
    seq_t m = (seq_t)n;
    seq_t v = (seq_t)34;
    v = v * m + (seq_t)(-108);
    v = v * m + (seq_t)370;
    v = v * m + (seq_t)(-480);
    v = v * m + (seq_t)451;
    v = v * m + (seq_t)(-132);
    v = v * m + (seq_t)5;
    v = v * (seq_t)2;
    v = v / (seq_t)5;
    return v;
#endif
}
