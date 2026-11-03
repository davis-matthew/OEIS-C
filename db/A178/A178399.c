/*
 * A178399  Primes p such that p is a quadratic residue modulo reverse(p) and
 *          reverse(p) is prime and a quadratic residue modulo p.
 * https://oeis.org/A178399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the n-th prime p such that: * 1) rev(p) is also prime, *
 *            2) p is a quadratic residue modulo rev(p), * 3) rev(p) is a
 *            quadratic residue modulo p. * The sequence is computed by
 *            searching primes and testing the * conditions using
 *            jacobi_symbol. Indexing starts at 1
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   is_prime, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>          /* for NULL */

#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

static seq_t *seq = NULL;          /* stored terms of the sequence   */
static size_t seq_len = 0;         /* number of terms stored         */
static size_t seq_cap = 0;         /* capacity of seq array          */
static uint64_t last_checked = 2;  /* next candidate prime to test   */

static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, seq, new_cap);
    if (!tmp) { /* allocation failed; abort */
        abort();
    }
    seq = tmp;
    seq_cap = new_cap;
}

static bool is_residue(uint64_t a, uint64_t p)
{
    /* Legendre symbol (a|p) for odd prime p.
     * jacobi_symbol returns 0 if a is divisible by p,
     * 1 if a is a quadratic residue, -1 otherwise. */
    int v = jacobi_symbol(a, p);
    return v == 1;
}

static void generate_to(idx_t n)
{
    while (seq_len < n) {
        /* Find next prime p */
        for (uint64_t p = last_checked; ; ++p) {
            if (!is_prime(p)) continue;
            last_checked = p + 1;

            /* Reverse */
            uint64_t r = reverse_digits_idx(p);
            if (r == p) continue;           /* skip palindromic primes */
            if (!is_prime(r)) continue;

            /* GCD must be 1; for primes this holds */
            /* Check quadratic residue conditions */
            if (is_residue(r, p) && is_residue(p, r)) {
                ensure_capacity(seq_len + 1);
                seq[seq_len++] = (seq_t)p;
                break;                      /* found next term of sequence */
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but guard */
    generate_to(n);
    return seq[n-1];
}
