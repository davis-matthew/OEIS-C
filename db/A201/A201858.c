/*
 * A201858  Primes of the form 8n^2 - 7.
 * https://oeis.org/A201858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 8*n^2 - 7 (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 2693, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* small prime test for 64‑bit numbers, used only as a speed boost */
static bool is_prime_seq(seq_t x)
{
    /* the numbers we test are < 10^9, fit easily in 64 bits */
    unsigned long long n = (unsigned long long)x;
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2;
    for (unsigned long long d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL)
            return false;
    return true;
}

/* public interface */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n<1; return 0 as a safe fall‑back */
        return 0;
    idx_t count = 0;       /* how many qualifying primes found */
    idx_t cand   = 0;       /* current n in 8*n^2 - 7 */
    seq_t val    = 0;       /* value of 8*cand^2 - 7 */

    while (count < n) {
        ++cand;
        val = (seq_t)8 * (seq_t)cand * (seq_t)cand - (seq_t)7;
        if (is_prime_seq(val))
            ++count;
    }
    return val;            /* val is now the n‑th prime of the required form */
}
