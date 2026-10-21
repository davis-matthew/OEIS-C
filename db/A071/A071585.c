/*
 * A071585  Numerator of the continued fraction expansion whose terms are the
 *          first-order differences of exponents in the binary representation
 *          of 4*n, with the exponents of 2 being listed in descending order.
 * https://oeis.org/A071585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2390, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive definition of A071585:
   a(0) = 1
   a(2^k) = k + 2
   For n = 2^k + 2^j + m with 2^k > 2^j > m >= 0,
   a(n) = (k - j) * a(2^j + m) + a(m).   */
#include <stdint.h>
#include <stdbool.h>

/* Compute floor(log2(x)) for x > 0.  Built‑in since the macro is
   available in GCC/Clang and works in C++17. */
static inline unsigned floor_log2_uint64(uint64_t x)
{
    /* x is guaranteed to be non‑zero by callers */
    return 63 - __builtin_clzll(x);
}

/* Test whether x is a power of two. */
static inline bool is_power_of_two(uint64_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

/* Sequence A071585 */
seq_t a(idx_t n)
{
    /* Handle the only base case directly. */
    if (n == 0)
        return (seq_t)1;

    /* Power‑of‑two values are known in closed form. */
    if (is_power_of_two((uint64_t)n)) {
        unsigned k = floor_log2_uint64((uint64_t)n);
        return (seq_t)k + 2;
    }

    /* General case: decompose n = 2^k + 2^j + m,
       with 2^k > 2^j > m >= 0. */
    unsigned k = floor_log2_uint64((uint64_t)n);
    uint64_t two_k = ((uint64_t)1) << k;

    uint64_t r = (uint64_t)n - two_k;          /* r = 2^j + m  (r > 0) */
    unsigned j = floor_log2_uint64(r);
    uint64_t two_j = ((uint64_t)1) << j;

    uint64_t m = r - two_j;                    /* m < 2^j */

    seq_t a_r = a((idx_t)r);   /* a(2^j + m) */
    seq_t a_m = a((idx_t)m);   /* a(m) */

    return ((seq_t)(k - j)) * a_r + a_m;
}
