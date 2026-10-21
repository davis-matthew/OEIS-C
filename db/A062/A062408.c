/*
 * A062408  Numbers k such that floor(Pi*k) is prime.
 * https://oeis.org/A062408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062408: a(n) is the nth positive integer k such that floor(π·k) is prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- 64‑bit primality test (deterministic Miller–Rabin) ---------- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    /* quick trial division */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
                                    23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    /* write n-1 as d·2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }
    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        /* compute a^d mod n */
        uint64_t x = 1;
        uint64_t base = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1ULL) x = (uint64_t)((__uint128_t)x * base % n);
            base = (uint64_t)((__uint128_t)base * base % n);
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = (uint64_t)((__uint128_t)x * x % n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------  integer floor of π·k  ---------- */
#define SCALE 1000000000000000000ULL          /* 10^18 */
#define PI_SCAL 3141592653589793238ULL        /* floor(π·10^18) */

static inline uint64_t floor_pi_times_uint64(uint64_t k)
{
    __uint128_t prod = (__uint128_t)PI_SCAL * k;
    return (uint64_t)(prod / SCALE);
}

/* ----------  dynamic cache of the sequence  ---------- */
static seq_t *seq = NULL;      /* array of computed a(n) values */
static size_t seq_len = 0;     /* number of stored values */
static idx_t next_k = 1;       /* next k to test for primality */

/* append a value to the cached array, growing it as needed */
static void append(seq_t val)
{
    if (seq_len == 0) {
        seq = (seq_t *)malloc(64 * sizeof(seq_t));
    } else if (seq_len & 63) {   /* grow when a multiple of 64 is reached */
        seq = (seq_t *)realloc(seq, (seq_len + 64) * sizeof(seq_t));
    }
    seq[seq_len++] = val;
}

/* ----------  required function  ---------- */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n+1 values */
    while (seq_len <= (size_t)n) {
        uint64_t k = (uint64_t)next_k;
        uint64_t p = floor_pi_times_uint64(k);
        while (!is_prime_u64(p)) {
            ++k;
            p = floor_pi_times_uint64(k);
        }
        append((seq_t)k);          /* store k as a(n) */
        next_k = k + 1;            /* continue from the next integer after k */
    }
    return seq[(size_t)n];
}
