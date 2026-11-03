/*
 * A104319  Primes which are the reverse concatenation of four consecutive
 *          cubes.
 * https://oeis.org/A104319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* Compute x^3 using seq_t arithmetic. */
static seq_t cube(seq_t x)
{
    return x * x * x;
}

/* Return the number of decimal digits of x. */
static idx_t digit_len(seq_t x)
{
    idx_t len = 0;
    seq_t t = x;
    while (t > 0) {
        t /= 10;
        len++;
    }
    return len;
}

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

/* Miller–Rabin primality test for arbitrary-precision values. */
static bool isprime_big(seq_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    idx_t r = 0;
    while ((d & 1) == 0) {
        d = d / 2;
        r = r + 1;
    }

    /* Bases sufficient for a probabilistic test. */
    const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29};
    for (idx_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        seq_t a = (seq_t)bases[i];
        if (a >= n) break;

        /* modular exponentiation x = a^d mod n */
        seq_t x = 1;
        seq_t base = a % n;
        seq_t exp  = d;
        while (exp > 0) {
            if ((exp & 1) != 0) x = (x * base) % n;
            base = (base * base) % n;
            exp  = exp >> 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (idx_t j = 1; j < r; ++j) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Sequence: primes obtained by concatenating four consecutive cubes in reverse order. */
seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static idx_t cap = 0;
    static idx_t size = 0;
    static idx_t next_k = 1;

    while (size < n) {
        seq_t c0 = cube((seq_t)next_k);
        seq_t c1 = cube((seq_t)(next_k + 1));
        seq_t c2 = cube((seq_t)(next_k + 2));
        seq_t c3 = cube((seq_t)(next_k + 3));

        idx_t len0 = digit_len(c0);
        idx_t len1 = digit_len(c1);
        idx_t len2 = digit_len(c2);
        idx_t len3 = digit_len(c3);

        seq_t candidate = c3;
        candidate = candidate * pow10(len2) + c2;
        candidate = candidate * pow10(len1) + c1;
        candidate = candidate * pow10(len0) + c0;

        if (isprime_big(candidate)) {
            push_seq(&primes, &cap, &size, candidate);
        }
        ++next_k;
    }

    return primes[n - 1];
}
