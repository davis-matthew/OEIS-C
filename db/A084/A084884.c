/*
 * A084884  Triangular array, read by rows: T(n,k) = numerator of arithmetic
 *          derivative of n/k, 1<=k<=n.
 * https://oeis.org/A084884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 76, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   index_to_rowcol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = numerator of the arithmetic derivative of n/k where
 * the pair (n,k) is the nth cell in the triangular array read by rows.
 * The triangular array is defined by T(n,k) = numerator of
 * (k*A003415(n)-n*A003415(k))/k^2, with A003415(m) the arithmetic
 * derivative of m.  The index n runs from 1 upwards.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/index_to_rowcol.h"

/* Small list of primes sufficient for numbers up to 200. */
static const uint64_t primes[] = {
    2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull,
    37ull, 41ull, 43ull, 47ull, 53ull, 59ull, 61ull, 67ull, 71ull, 73ull,
    79ull, 83ull, 89ull, 97ull, 101ull, 103ull, 107ull, 109ull, 113ull,
    127ull, 131ull, 137ull, 139ull, 149ull, 151ull, 157ull, 163ull,
    167ull, 173ull, 179ull, 181ull, 191ull, 193ull, 197ull, 199ull
};
static const size_t PRIME_CNT = sizeof(primes)/sizeof(primes[0]);

/* Greatest common divisor for signed integer types. */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Arithmetic derivative of a small integer m (m <= 200). */
static seq_t arithmetic_derivative(idx_t m)
{
    if (m <= 1) return 0;
    uint64_t n = (uint64_t)m;
    seq_t result = 0;
    for (size_t i = 0; i < PRIME_CNT && primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            seq_t contrib = (seq_t)((uint64_t)m / p) * (seq_t)e;
            result += contrib;
        }
    }
    if (n > 1) {   /* remaining prime factor */
        seq_t contrib = (seq_t)((uint64_t)m / n);
        result += contrib;
    }
    return result;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    idx_t r, c;
    index_to_rowcol(n, &r, &c);

    seq_t dr = arithmetic_derivative(r);
    seq_t dk = arithmetic_derivative(c);

    seq_t num = (seq_t)c * dr - (seq_t)r * dk;
    seq_t den = (seq_t)c * (seq_t)c;

    if (num == 0) return 0;

    seq_t g = gcd_seq(num < 0 ? -num : num, den);
    return num / g;
}
