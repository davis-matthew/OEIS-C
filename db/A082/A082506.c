/*
 * A082506  a(n) = gcd(2^n, n - phi(n)); largest power of 2 dividing
 *          cototient(n) = A051953(n).
 * https://oeis.org/A082506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest power of 2 dividing n - φ(n)
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --- local totient calculation --------------------------------------- */
/* Computes φ(n) for an unsigned 64‑bit integer n.  The result fits in
   the same type as n, so it can be cast to seq_t without loss.  */
static seq_t phi_uint64(idx_t n)
{
    idx_t original = n;
    seq_t result = n;

    for (idx_t p = 2; p * (idx_t)p <= n; ++p) {
        if (n % p == 0) {
            do { n /= p; } while (n % p == 0);
            result = result / p * (p - 1);
        }
    }
    if (n > 1) {
        result = result / n * (n - 1);
    }

    return result;
}

/* -------------------------------------------------------------------- */
/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Special case: n = 1 gives n - φ(n) = 0, so gcd(2¹, 0) = 2¹ = 2. */
    if (n == 1) return (seq_t)2;

    /* Compute n - φ(n).  Visible as an unsigned 64‑bit integer. */
    seq_t phi = phi_uint64(n);
    idx_t diff = n - (idx_t)phi;      /* diff > 0 for n > 1 */

    /* Count trailing zeros of diff: exponent of 2 dividing it. */
    int tz = __builtin_ctzll((unsigned long long)diff);

    /* Return 2^tz.  tz ≤ 63, so the value fits in seq_t for any build. */
    return ((seq_t)1) << tz;
}
