/*
 * A089348  Primes of the form smallest multiple of n followed by a 1.
 * https://oeis.org/A089348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the smallest prime of the form 10*m + 1 where m is a
 *            multiple of n. * * a(n) = min{ 10*(n*k) + 1 : k ≥ 1 and
 *            10*(n*k)+1 is prime }. * * This computes the sequence for n ≥ 1
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime table (primes up to 40000) ---------- */
static bool prime_table_ready = false;
static size_t prime_count = 0;
static uint32_t primes[4205];   /* enough for primes ≤ 40000 */

/* Initialise the prime list once. */
static void init_prime_table(void)
{
    if (prime_table_ready) return;

    const int LIMIT = 40000;
    for (int i = 2; i <= LIMIT; ++i) {
        bool pc = true;
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = primes[j];
            if ((uint32_t)p * (uint32_t)p > (uint32_t)i) break;
            if (i % p == 0) { pc = false; break; }
        }
        if (pc) primes[prime_count++] = (uint32_t)i;
    }
    prime_table_ready = true;
}

/* ---------- 64‑bit primality test using the small prime list ---------- */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > x) return true;            /* no divisor found */
        if (x % p == 0) return false;       /* divisible */
    }
    return true;                            /* prime */
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* offset starts at 1 */

    init_prime_table();

    seq_t result = 0;
    idx_t k = 1;
    while (true) {
        /* m = n*k, candidate prime = 10*m + 1 */
        seq_t m = (seq_t)n * (seq_t)k;
        seq_t p = m * (seq_t)10 + (seq_t)1;

        /* test primality (value small enough to cast to 64‑bit) */
        uint64_t p_val = (uint64_t)p;
        if (is_prime_uint64(p_val)) {
            result = p;
            break;
        }
        ++k;
    }
    return result;
}
