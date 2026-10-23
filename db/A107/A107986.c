/*
 * A107986  Composite numbers of the form p+2 where p is prime.
 * https://oeis.org/A107986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = composite numbers of the form p+2 where p is prime
 *            (1‑based)
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---- minimal deterministic Miller–Rabin for 64‑bit unsigned integers ---- */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((__uint128_t)a * b % m);
}

static uint64_t pow_mod_uint64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod_uint64(r, a, m);
        a = mul_mod_uint64(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1; int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        if (bases[i] % n == 0) continue;
        uint64_t x = pow_mod_uint64(bases[i], d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, OK for any offset */
    uint64_t count = 0;
    /* handle p = 2 separately (even prime) */
    if (!is_prime_uint64(4)) {    /* 4 is composite */
        ++count;
        if (count == n) return (seq_t)4;
    }
    /* iterate odd primes p >= 3 */
    uint64_t p = 3;
    for (;;) {
        if (is_prime_uint64(p)) {
            uint64_t q = p + 2;
            if (!is_prime_uint64(q)) {
                ++count;
                if (count == n) return (seq_t)q;
            }
        }
        p += 2;                    /* skip even numbers */
    }
}
