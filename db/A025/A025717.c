/*
 * A025717  Index of 6^n within the sequence of the numbers of the form
 *          6^i*8^j.
 * https://oeis.org/A025717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1..n} ⌈k·c⌉, where c = log₈(6) = log₂(6)/3
 * Range:     u8 n <= 23, u16 n <= 388, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The ceil is computed using a fixed‑point integer approximation of c.   */

/* Rational approximation of c with denominator 10¹⁸.
   NUM64 / DEN64 ≈ c  with an error < 5·10⁻¹⁹.  */
static const uint64_t NUM64 = 861654166907052070ULL;
static const uint64_t DEN64 = 1000000000000000000ULL;

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;               /* a(0) = 1 */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute ceil(k * NUM64 / DEN64) using 128‑bit arithmetic. */
        __int128 prod       = (__int128)k * NUM64;
        uint64_t ceil_kc    = (uint64_t)((prod + (__int128)DEN64 - 1) / (__int128)DEN64);
        result += (seq_t)ceil_kc;
    }
    return result;
}
