/*
 * A049593  Primes p for which residue of ((p-1)! + 1) modulo (p + 16) equals
 *          1.
 * https://oeis.org/A049593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 5662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: prime p such that (p-1)! ≡ 0 (mod p+16)  */
/* Uses trial division to test the condition for each prime p.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Append value to a growing array of seq_t. */
static void push_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        seq_t *new_arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = new_arr;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = val;
}

/* True if prime p satisfies (p-1)! ≡ 0 (mod p+16). */
static bool good_prime(idx_t p)
{
    idx_t m = p + 16;
    idx_t tmp = m;

    for (idx_t f = 2; f * f <= tmp; ++f) {
        if (tmp % f) continue;
        int exp = 0;
        while (tmp % f == 0) { tmp /= f; ++exp; }

        /* exponent of f in (p-1)! via Legendre's formula */
        idx_t v = 0;
        idx_t t = p - 1;
        while (t) { t /= f; v += t; }
        if (v < (idx_t)exp) return false;
    }
    if (tmp > 1) {          /* remaining prime factor */
        idx_t f = tmp;
        int exp = 1;
        idx_t v = 0;
        idx_t t = p - 1;
        while (t) { t /= f; v += t; }
        if (v < (idx_t)exp) return false;
    }
    return true;
}

/* Cached sequence values. */
static seq_t   *values = NULL;
static size_t  values_len = 0;
static size_t  values_cap = 0;
static idx_t   last_checked = 1;   /* last number examined for being a qualifying prime */

/* Ensure sequence contains at least n terms (1‑based). */
static void grow_sequence(idx_t n)
{
    while (values_len < (size_t)n) {
        idx_t p = next_prime(last_checked);
        last_checked = p;
        if (is_prime(p) && good_prime(p))
            push_seq(&values, &values_len, &values_cap, (seq_t)p);
    }
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    grow_sequence(n);
    return values[(size_t)n - 1];
}
