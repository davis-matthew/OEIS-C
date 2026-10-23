/*
 * A014948  Numbers k such that k divides s(k), where s(1)=1, s(j)= s(j-1) +
 *          j*7^(j-1).
 * https://oeis.org/A014948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A014948: numbers k such that k divides s(k), where
   s(1)=1 and s(j)=s(j-1)+j*7^(j-1).
   Equivalent condition: 1 + (6k-1)*7^k ≡ 0 (mod 36k). */

/* The function builds the sequence on demand and caches the found
   terms so that calls with arbitrary n are fast. */
#include <stdbool.h>
#include <stdlib.h>

/* Multiply two seq_t values modulo m using the binary method
   (safe for all supported seq_t sizes). */
static seq_t mul_mod_small(seq_t a, seq_t b, seq_t m)
{
    seq_t res = 0;
    a %= m;
    b %= m;
    while (b) {
        if (b & 1) {
            res += a;
            if (res >= m) res -= m;
        }
        a += a;
        if (a >= m) a -= m;
        b >>= 1;
    }
    return res;
}

/* Compute (base^exp) mod m.  exp is given as idx_t (64‑bit). */
static seq_t pow_mod(seq_t base, idx_t exp, seq_t m)
{
    seq_t result = 1 % m;
    seq_t power  = base % m;
    while (exp) {
        if (exp & 1) result = mul_mod_small(result, power, m);
        power = mul_mod_small(power, power, m);
        exp >>= 1;
    }
    return result;
}

/* Test whether a given k satisfies the divisibility condition. */
static bool k_divides_s_k(idx_t k)
{
    seq_t k_seq = (seq_t)k;
    seq_t mod   = (seq_t)36 * k_seq;              /* 36*k */
    seq_t pow   = pow_mod((seq_t)7, k, mod);      /* 7^k mod (36k) */

    /* a_val = (6*k - 1) mod mod */
    seq_t a_val = mul_mod_small((seq_t)6, k_seq, mod);
    a_val = (a_val + mod - 1) % mod;

    seq_t mul = mul_mod_small(a_val, pow, mod);
    seq_t num = mul + 1;
    if (num >= mod) num -= mod;
    return num == 0;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *vals   = NULL;
    static size_t count  = 0;
    static size_t capacity = 0;
    static idx_t next_k = 1;            /* smallest k not yet tested */

    /* Grow cache when needed */
    if (count >= capacity) {
        size_t new_cap = capacity ? capacity << 1 : 32;
        seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        vals = tmp;
        capacity = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (count < n) {
        if (k_divides_s_k(next_k)) {
            vals[count++] = (seq_t)next_k;
        }
        ++next_k;
        if (count >= capacity) {
            size_t new_cap = capacity ? capacity << 1 : 32;
            seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            vals = tmp;
            capacity = new_cap;
        }
    }

    return vals[n - 1];
}
