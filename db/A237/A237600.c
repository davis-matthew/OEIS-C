/*
 * A237600  Right-truncatable primes in base 16.
 * https://oeis.org/A237600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 119, u32 n <= 372, u64 n <= 414,
 *            u128 n <= 414, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Right‑truncatable primes in base 16 (OEIS A237600), computed by breadth‑first search.
   The sequence is built once, then cached. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

/* -----------------  64‑bit primality test ----------------- */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Miller–Rabin deterministic for 64‑bit integers */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, base = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * base) % n;
            base = (base * base) % n;
            e >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* -----------------  Sequence storage ----------------- */
static seq_t seq_arr[500];   /* 500 > maximum number (414) of terms */
static size_t seq_len = 0;
static bool built = false;

/* Return true if val fits in 64 bits; otherwise false.
   (Needed for safety when seq_t may be >64 bits.) */
static inline bool fits_in_uint64(seq_t val)
{
    uint64_t mask = (uint64_t)val;
    return (seq_t)mask == val;
}

/* Build the whole sequence once */
static void build_sequence(void)
{
    /* primes < 16 */
    const uint64_t small[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        seq_arr[seq_len++] = (seq_t)small[i];
    }

    /* maximum value representable in seq_t */
    seq_t max_val = (seq_t)(~(seq_t)0);

    size_t idx = 0;
    while (idx < seq_len) {
        seq_t p = seq_arr[idx++];
        for (uint8_t d = 0; d < 16; ++d) {
            __uint128_t q128 = (__uint128_t)p * 16 + d;
            if (q128 > (__uint128_t)max_val) continue;   /* would overflow seq_t */
            seq_t q = (seq_t)q128;
            if (!fits_in_uint64(q)) continue;            /* too big for our primality test */
            if (is_prime_uint64((uint64_t)q)) {
                if (seq_len < sizeof(seq_arr)/sizeof(seq_arr[0]))
                    seq_arr[seq_len++] = q;
            }
        }
    }
    built = true;
}

/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!built) build_sequence();
    if (n == 0) return (seq_t)0;            /* undefined, return 0 */
    if ((size_t)(n - 1) < seq_len) return seq_arr[n - 1];
    return (seq_t)0;                          /* out of range (no more terms) */
}
