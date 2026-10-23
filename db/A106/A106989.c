/*
 * A106989  Primes of the form x^2-xy+23y^2, with x and y nonnegative.
 * https://oeis.org/A106989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p > 1 that can be written as x^2 - x*y +
 *            23*y^2 (equivalently, primes with Legendre symbols (p|7) =
 *            (p|13) = 1)
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation scans primes in increasing order, and keeps those
   for which both Legendre symbols modulo 7 and 13 are +1.  No other
   prerequisites are used. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- fast modular exponentiation ---------------------------------- */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------- Legendre symbol for odd prime p (p | 7 or 13) ---------------- */
static bool legendre_residue(uint64_t p, uint32_t prime)
{
    uint64_t a = p % prime;
    if (a == 0) return false;    /* divisible, symbol = 0 */
    uint64_t ls = mod_pow_uint64(a, (prime - 1) >> 1, prime);
    return ls == 1;
}

/* ---------- deterministic Miller–Rabin for 64‑bit integers --------------- */
static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pow = a, e = d;
        while (e) {
            if (e & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- next prime after ‘start’ (start < prime) --------------------- */
static uint64_t next_prime_from(uint64_t start)
{
    uint64_t p = (start <= 2) ? 2 : (start | 1);
    for (;;) {
        if (p >= 3 && !(p & 1)) { p += 2; continue; }
        if (is_prime64(p)) return p;
        p += (p == 2) ? 1 : 2;
    }
}

/* ---------- static storage for generated terms -------------------------- */
static seq_t *seq_vals = NULL;
static idx_t seq_cnt = 0, seq_cap = 0;

static void seq_expand(void)
{
    if (seq_cap == 0) {
        seq_cap = 16;
        seq_vals = (seq_t *)malloc(seq_cap * sizeof *seq_vals);
    } else {
        seq_cap <<= 1;
        seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof *seq_vals);
    }
}

/* ---------- the requested function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* harmless placeholder */
    static uint64_t last_checked = 1;   /* up to 1, so first prime is 2 */
    while (seq_cnt < n) {
        uint64_t p = next_prime_from(last_checked + 1);
        last_checked = p;
        if (legendre_residue(p, 7) && legendre_residue(p, 13)) {
            if (seq_cnt == seq_cap) seq_expand();
            seq_vals[seq_cnt++] = (seq_t)p;
        }
    }
    return seq_vals[n - 1];
}
