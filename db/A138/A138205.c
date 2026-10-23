/*
 * A138205  Least number k such that n^2+k and n^2+k+2 are the smallest twin
 *          primes between squares n^2 and (n+1)^2, or 0 if there is no such
 *          k.
 * https://oeis.org/A138205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with n^2 + k and n^2 + k + 2 the smallest
 *            twin–prime pair between n^2 and (n+1)^2; a(n)=0 if no such pair
 *            exists
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* 64‑bit modular multiplication using __int128 (C99/C++17 compatible) */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation */
static uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers.
   Witnesses {2,3,5,7,11,13} are sufficient for all 64‑bit numbers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;

    /* trivial small primes */
    static const uint64_t small[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
         23ULL, 29ULL, 31ULL, 37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t witnesses[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i];
        if (a % n == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for the interval bounds */
    uint64_t low  = (uint64_t)n * (uint64_t)n;
    uint64_t high = (uint64_t)(n + 1) * (uint64_t)(n + 1);

    uint64_t start = low + 1;
    if ((start & 1) == 0) start++;        /* start at the first odd >= low+1 */

    for (uint64_t p = start; p + 2 < high; p += 2) {
        if (is_prime_u64(p) && is_prime_u64(p + 2))
            return (seq_t)(p - low);
    }
    return (seq_t)0;
}
