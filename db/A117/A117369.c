/*
 * A117369  a(n) = smallest prime which is > smallest prime dividing n and is
 *          coprime to n.
 * https://oeis.org/A117369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p > smallest_prime_factor(n)
 *            and p is coprime to n (i.e., p does not divide n). The
 *            implementation uses a simple trial‑division for primality and
 *            smallest prime factor
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t i = 3ULL; i <= n / i; i += 2ULL) {
        if (n % i == 0ULL) return i;
    }
    return n;  // n is prime
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2ULL) return false;
    if (n == 2ULL) return true;
    if (n % 2ULL == 0ULL) return false;
    for (uint64_t i = 3ULL; i <= n / i; i += 2ULL) {
        if (n % i == 0ULL) return false;
    }
    return true;
}

static uint64_t next_prime_after(uint64_t x)
{
    uint64_t candidate = x + 1ULL;
    if (candidate <= 2ULL) return 2ULL;
    if (candidate % 2ULL == 0ULL) candidate++;
    for ( ;; candidate += 2ULL ) {
        if (is_prime_uint64(candidate)) return candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but safe */
    if (n == 1) return (seq_t)2;      /* special case from the sequence */

    uint64_t nn = (uint64_t)n;
    uint64_t spf = smallest_prime_factor(nn);
    uint64_t p = next_prime_after(spf);

    /* find the first prime > spf that does not divide n */
    while (nn % p == 0ULL) {
        p = next_prime_after(p);
    }
    return (seq_t)p;
}
