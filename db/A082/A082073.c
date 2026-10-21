/*
 * A082073  First difference set of primes with 4k+1 form: A002144.
 * https://oeis.org/A082073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002144(n+1) – A002144(n), i.e. the difference between
 *            successive primes of the form 4k+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine lazily generates primes of that form and keeps them in a static array. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Miller‑Rabin primality test for 64‑bit integers ---------- */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for 64‑bit integers. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a, mod = n;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * p) % mod;
            p = (p * p) % mod;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % mod;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Static storage for primes ≡ 1 (mod 4) ---------- */
static uint64_t *p1_mod4 = NULL;   /* array of primes, 0‑based: p1_mod4[0] = 5   */
static size_t  p1_len  = 0;        /* current number of stored primes            */
static size_t  p1_cap  = 0;        /* current capacity of the array              */

/* Push a new prime into the static array, reallocating if necessary. */
static void push_prime(uint64_t prime)
{
    if (p1_len == p1_cap) {
        size_t new_cap = (p1_cap == 0) ? 16 : p1_cap * 2;
        uint64_t *tmp = (uint64_t *)realloc(p1_mod4, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        p1_mod4 = tmp;
        p1_cap = new_cap;
    }
    p1_mod4[p1_len++] = prime;
}

/* Ensure that at least `needed` primes of form 4k+1 are stored (0‑based index). */
static void ensure_primes(idx_t needed)
{
    if (needed <= p1_len) return;          /* already have enough */

    /* initialise with the first such prime if the table is empty */
    if (p1_len == 0) {
        push_prime(5);
    }

    /* generate primes until we reach the required count */
    uint64_t candidate = p1_mod4[p1_len - 1] + 4;  /* next number ≡ 1 mod 4 */
    while (p1_len < needed) {
        if (is_prime_u64(candidate)) {
            push_prime(candidate);
        }
        candidate += 4;
    }
}

/* ---------- The required OEIS function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, but guard against underflow */

    /* a(1) = p2 − p1, so we need primes up to index n (0‑based) + 1. */
    idx_t required = n + 1;
    ensure_primes(required);

    /* compute difference using seq_t arithmetic */
    seq_t diff = (seq_t)p1_mod4[(size_t)n] - (seq_t)p1_mod4[(size_t)(n - 1)];
    return diff;
}
