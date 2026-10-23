/*
 * A141277  a(n) = k(n) mod ((sum of prime factors of k(n))+1), where k(n) =
 *          n-th composite.
 * https://oeis.org/A141277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k(n) mod ((sum of prime factors of k(n))+1) where k(n) is
 *            the n‑th composite number
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test if a number is prime (trial division).  Works for the size of numbers
   we encounter (n up to about 10 000 → k(n) < 20 000). */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* Sum prime factors of m (with multiplicity).  Uses trial division. */
static uint64_t sum_prime_factors(uint64_t m)
{
    uint64_t sum = 0;
    uint64_t n = m;
    while ((n & 1ULL) == 0) {          /* factor 2 while possible */
        sum += 2;
        n >>= 1;
    }
    for (uint64_t p = 3; p <= n / p; p += 2) {
        while (n % p == 0) {
            sum += p;
            n /= p;
        }
    }
    if (n > 1) sum += n;                /* remaining prime factor */
    return sum;
}

/* -------------------------------------------------------------
   Main sequence function
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for under‑flow, though n ≥ 1 by definition */

    /* Find the nth composite number. */
    uint64_t count = 0;
    uint64_t num = 4;          /* first composite */
    while (1) {
        if (!is_prime(num)) {
            count++;
            if (count == n) break;
        }
        num++;
    }
    uint64_t k = num;                 /* k(n) */

    /* Compute sum of prime factors and take modulus. */
    uint64_t sum = sum_prime_factors(k);
    uint64_t mod_base = sum + 1;
    seq_t result = (seq_t)(k % mod_base);

    return result;
}
