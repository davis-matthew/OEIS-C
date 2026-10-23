/*
 * A016804  a(n) = (4*n)^4.
 * https://oeis.org/A016804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)^4
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 63, u64 n <= 600, u128 n <= 600,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = 256 * n^4 * 1                       */

/* a(n) is defined for all non‑negative n.  The value is
 * computed exactly for all built‑in integer types that
 * support 128‑bit arithmetic; otherwise it falls back to
 * an intermediate of type seq_t.  For arbitrary‑precision
 * seq_t types the natural multiplication already yields
 * an exact value.  Only the cast to seq_t occurs at the end
 * of the function. */
seq_t a(idx_t n)
{
#ifdef __SIZEOF_INT128__
    /* If the compiler provides a native 128‑bit type use it
       to avoid overflow while computing 256*n^4. */
    __uint128_t x = ( __uint128_t )n;   /* n            */
    x *= x;                  /* n^2          */
    x *= x;                  /* n^4          */
    x *= 256;                /* 256*n^4      */
    return (seq_t)x;
#else
    /* Fallback for platforms without __int128.  For built‑in
       types that cannot hold the full value the result may
       overflow, but this is fine because the caller only
       checks values that fit in seq_t. */
    seq_t t = (seq_t)n;       /* n            */
    t = t * t;                /* n^2          */
    t = t * t;                /* n^4          */
    t = t * (seq_t)256;       /* 256*n^4      */
    return t;
#endif
}
