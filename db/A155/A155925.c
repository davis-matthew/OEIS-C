/*
 * A155925  a(x) = if one of {4x^2 - 146x + 1373, 4x^2 - 144x + 1459, 4x^2 -
 *          142x + 1301, 4x^2 - 140x + 1877} is prime, then pick that prime in
 *          sequence, otherwise pick zero.
 * https://oeis.org/A155925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 145, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155925: test the four quadratic polynomials cyclically.
   Return the first prime found, or 0 if none are prime. */
/* The polynomials are:
   p0(n) = 4n^2 - 146n + 1373
   p1(n) = 4n^2 - 144n + 1459
   p2(n) = 4n^2 - 142n + 1301
   p3(n) = 4n^2 - 140n + 1877
   The test starts at index n mod 4 and continues in order. */
#include <stdbool.h>
#include <stdint.h>

/* Simple deterministic primality test for numbers up to approx 4·10^12.
   It is safe for the typical values produced by the polynomials
   (for n up to about 100000). For larger values the caller
   should provide a more sophisticated test. */
static bool is_prime_seq(seq_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    /* loop i from 3 to sqrt(v) step 2 */
    for (seq_t i = 3; i * i <= v; i += 2) {
        if (v % i == 0) return false;
    }
    return true;
}

static const int64_t A[4] = { -146, -144, -142, -140 };
static const int64_t B[4] = { 1373, 1459, 1301, 1877 };

seq_t a(idx_t n)
{
    /* compute n^2 in signed 64-bit (safe for typical n). */
    int64_t nsq = (int64_t)n * (int64_t)n;
    int start = (int)(n % 4);
    for (int offset = 0; offset < 4; ++offset) {
        int idx = (start + offset) & 3;
        int64_t val64 = 4 * nsq + A[idx] * (int64_t)n + B[idx];
        /* val64 should be positive; cast to seq_t. */
        seq_t val = (seq_t)val64;
        if (is_prime_seq(val)) return val;
    }
    return (seq_t)0;
}
