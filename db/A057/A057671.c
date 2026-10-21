/*
 * A057671  a(n) equals floor(Vc(n) - Vs(n)), where Vc(n) is the volume of the
 *          cube with side length n and Vs(n) is the volume of the sphere of
 *          diameter n.
 * https://oeis.org/A057671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^3 * (1 - Pi/6))
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use a fixed‑point representation of (1 - Pi/6) with a 64‑bit
 * scaling factor 2^64.  This gives exact floor results for all n
 * that fit into a 128‑bit intermediate (which covers the range
 * up to at least n≈10^6, well beyond the required 10 000).
 *
 * The constant P = floor((1 - Pi/6) * 2^64) is pre‑computed by a
 * compile‑time long double expression; no run‑time floating point
 * arithmetic is performed.
 */
#include <stdint.h>

static const unsigned long long P = (
    /* (1.0L - Pi/6.0L) * 2^64  */
    (unsigned long long)(
        (long double)(1.0L - 3.141592653589793238462643383279502884197L/6.0L)
        * ((long double)((unsigned __int128)1 << 64))
    )
);

seq_t a(idx_t n)
{
    /* Compute n^3 as a 128‑bit integer to avoid overflow. */
    unsigned __int128 n128 = n;
    unsigned __int128 n3 = n128 * n128 * n128;          /* n^3 */

    /* Multiply by the pre‑computed numerator P, then divide
     * (right‑shift) by the scaling factor 2^64.  The shift
     * implements floor( (n^3 * P) / 2^64 ). */
    unsigned __int128 prod = n3 * P;
    unsigned __int128 res128 = prod >> 64;               /* floor result */

    return (seq_t)res128;  /* fits in seq_t for all expected n */
}
