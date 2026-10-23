/*
 * A108233  Numbers n such that 11*n + 5 is prime.
 * https://oeis.org/A108233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 5814, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A108233: a(n) is the nth non‑negative integer n such that 11*n + 5 is prime. */
/* The function searches sequentially for such n and returns the nth term. */

#include <stdint.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin test for 64‑bit integers (trial division for simplicity). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    while (i <= n / i) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, return 0 */
    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        uint64_t cand = 11ULL * (uint64_t)k + 5ULL;   /* 11*k + 5 */
        if (is_prime_uint64(cand)) {
            ++count;
            if (count == n) return (seq_t)k;          /* found the nth term */
        }
        ++k;
    }
}
