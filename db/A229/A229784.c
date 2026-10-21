/*
 * A229784  a(n) = (1^1^1 + 2^2^2 . . . + n^n^n) mod 10.
 * https://oeis.org/A229784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = (1^1^1 + 2^2^2 + ... + n^n^n) mod 10. */
/* Periodicity: the sequence modulo 10 repeats every 100 terms. */

#include <stddef.h>
#include <stdbool.h>

/* Cycle lengths of b^k modulo 10 for b = 0..9. */
static const int cycle_len[10] = { 1, 1, 4, 4, 2, 1, 1, 4, 4, 2 };

/* modular exponentiation for a small base and exponent, modulo 'mod'. */
static int powmod_small(int base, int exp, int mod)
{
    int result = 1 % mod;
    int b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (int)((long long)result * b % mod);
        b = (int)((long long)b * b % mod);
        exp >>= 1;
    }
    return result;
}

/* modular exponentiation modulo 10; works for any non‑negative exponent. */
static int powmod10(int base, int exp)
{
    int result = 1;
    int b = base % 10;
    while (exp > 0) {
        if (exp & 1)
            result = (result * b) % 10;
        b = (b * b) % 10;
        exp >>= 1;
    }
    return result;
}

/* Computes a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* static cache of the first 100 prefix sums modulo 10 */
    static seq_t prefix[100];
    static bool init = false;

    if (!init) {
        prefix[0] = 0;
        for (int k = 1; k < 100; ++k) {   /* compute up to k = 99 */
            int base = k % 10;
            int cyc  = cycle_len[base];
            int exp_mod;

            if (cyc == 1) {
                /* For digits with cycle length 1, exponent is irrelevant */
                exp_mod = 1;
            } else {
                /* Compute k^k modulo the cycle length */
                exp_mod = powmod_small(k % cyc, k, cyc);
                if (exp_mod == 0)
                    exp_mod = cyc;   /* 0 represents a multiple of the cycle */
            }

            /* last digit of base^{exp_mod} */
            int val = powmod10(base, exp_mod);

            prefix[k] = (prefix[k - 1] + (seq_t)val) % 10;
        }
        init = true;
    }

    /* The sequence is periodic with period 100. */
    return prefix[(size_t)(n % 100)];
}
