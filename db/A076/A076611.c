/*
 * A076611  Palindromic primes with prime middle digit.
 * https://oeis.org/A076611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_digit, rev_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A076611: palindromic primes with prime middle digit (odd length). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_digit.h"
#include "../utils/rev_digits_uint64.h"

/* ---------- helper functions ---------- */

/* Primitive modular multiplication using 128‑bit arithmetic. */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* Modular exponentiation. */
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

/* Deterministic Miller‑Rabin for 64‑bit integers. */
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

    /* Witnesses sufficient for all 64‑bit integers. */
    static const uint64_t witnesses[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof witnesses / sizeof *witnesses; ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- memoised sequence generator ---------- */

static seq_t *seq = NULL;          /* stored primes */
static size_t seq_count = 0;       /* number of stored primes */
static size_t seq_cap   = 0;       /* array capacity */
static int   next_len   = 1;       /* next palindrome length to generate */

/* Push a new value into the memoised array. */
static void push_seq_val(seq_t val)
{
    if (seq_count == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_count++] = val;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safe */

    /* generate primes until we have at least n terms */
    while (seq_count < (size_t)n) {
        int len  = next_len; /* odd length */
        int k    = (len - 1) / 2;      /* number of digits after the middle (excluding middle) */

        uint64_t pow10k = 1;
        for (int i = 0; i < k; ++i) pow10k *= 10;          /* 10^k */

        uint64_t start = pow10k;                             /* minimum left part (no leading zero) */
        uint64_t end   = pow10k * 10 - 1;                    /* maximum left part */

        for (uint64_t left = start; left <= end; ++left) {
            int mid = left % 10;
            if (!is_prime_digit(mid))
                continue;

            uint64_t rem = left / 10;                       /* left part without the middle digit */
            uint64_t rev = rev_digits_uint64(rem);
            uint64_t pal = left * pow10k + rev;

            if (is_prime64(pal))
                push_seq_val((seq_t)pal);
        }

        next_len += 2;   /* move to next odd length */
    }

    return seq[(size_t)n - 1];
}
