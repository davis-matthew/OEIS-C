/*
 * A106782  Primes with digit sum = 53.
 * https://oeis.org/A106782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime (index 1) whose decimal digit sum equals 53
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* modular multiplication using 128‑bit intermediate to avoid overflow */
static uint64_t modmul(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* modular exponentiation */
static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) {
            result = modmul(result, base, mod);
        }
        base = modmul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin primality test for 64‑bit values.
   Works for all n < 2^64 (citing known base set). */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    /* quick trial division by small primes */
    static const uint64_t small_primes[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
         23ULL, 29ULL, 31ULL, 37ULL};
    for (int i = 0; i < (int)(sizeof(small_primes)/sizeof(small_primes[0])); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if ((n % p) == 0) return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases that guarantee correctness for 64‑bit unsigned integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (int i = 0; i < (int)(sizeof(bases)/sizeof(bases[0])); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool passed = false;
        for (int r = 1; r < s; ++r) {
            x = modmul(x, x, n);
            if (x == n - 1) { passed = true; break; }
        }
        if (!passed) return false;
    }
    return true;
}

/* sum of decimal digits of a 64‑bit integer */
static uint64_t digit_sum_uint64(uint64_t n) {
    uint64_t sum = 0;
    while (n) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

/* sequence A106782 */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 – ignore n==0 */
    uint64_t count = 0;
    uint64_t candidate = 3;        /* start from the first odd prime > 2 */
    while (true) {
        if (digit_sum_uint64(candidate) == 53) {
            if (is_prime_uint64(candidate)) {
                ++count;
                if (count == n) return (seq_t)candidate;
            }
        }
        candidate += 2;           /* skip even numbers */
    }
}
