/*
 * A079450  a(n) = 2^(n-1)*u(n) where u(1)=1 and u(n) = frac(3/2*u(n-1)) + 1.
 * https://oeis.org/A079450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A079450 – a(n) = 2^(n-1)*u(n)
 * where u(1)=1 and u(n)=frac(3/2*u(n-1))+1.
 *
 * Recurrence that follows from the definition:
 *   a(1) = 1
 *   a(n) = (3*a(n-1) mod 2^(n-1)) + 2^(n-1)   (n ≥ 2)
 *
 * The implementation evaluates the recurrence in O(n) time per call
 * and works for any seq_t type that supports the usual arithmetic
 * and bitwise operations (including arbitrary‑precision integer
 * classes).  No dynamic memory allocation or caching is required,
 * thus the function is valid for both C11 and C++17 builds.
 */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1 – not expected to be called */
        return (seq_t)0;

    seq_t val = 1;         /* a(1) */

    if (n == 1)
        return val;

    for (idx_t i = 2; i <= n; ++i) {
        /* exponent for the current power of two */
        idx_t exp = i - 1;

        /* mod = 2^(i-1) */
        seq_t mod = 1;
        mod <<= exp;                 /* 2^(i-1) */

        /* remainder of 3*val modulo mod */
        seq_t r = (3 * val) & (mod - 1);

        /* a(i) = remainder + mod */
        val = r + mod;
    }

    return val;
}
