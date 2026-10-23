/*
 * A181943  Least prime p > 3 such that p > n (log p) log log p.
 * https://oeis.org/A181943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A181943
 * Least prime p > 3 such that
 *    p ≥ n   and   p > n * (log p) * log(log p)
 * (log denotes the natural logarithm).
 *
 * The implementation iterates over successive odd integers
 * starting from max(5, n).  For each prime found it evaluates
 * the inequality using double precision logarithms.
 * The first prime satisfying both conditions is returned.
 * The code uses only integer arithmetic on the seq_t values
 * and relies on a small helper to test primality.
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

/* Simple trial‑division primality test for uint64_t values. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Ensure that n is at least 1 (the sequence is defined for n ≥ 1). */
    if (n < 1) n = 1;

    /* Start searching from the smallest prime > 3 that is at least n. */
    uint64_t candidate = (n > 5) ? n : 5ULL;

    /* Make sure the starting candidate is odd (primes are odd except 2). */
    if (candidate <= 2) candidate = 3;
    if (candidate % 2 == 0) candidate += 1;

    for (;;) {
        if (is_prime_uint64(candidate)) {
            double pd  = (double)candidate;
            double rhs = (double)n * log(pd) * log(log(pd));
            if (pd > rhs) {
                return (seq_t)candidate;
            }
        }
        /* Advance to the next odd integer. */
        if (candidate <= 3)
            candidate = 5;
        else
            candidate += 2;
    }
}
