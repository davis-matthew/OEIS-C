/*
 * A122907  Numbers m such that in Z/mZ the number of squares is greater than
 *          or equal to the number of invertible elements.
 * https://oeis.org/A122907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   NUMBER(m) = 1 if m is 1 else number of square residues in Z/mZ
 *            >= φ(m)
 * Range:     u8 n <= 57, u16 n <= 4942, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   squares_for_2pow, squares_for_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/squares_for_2pow.h"
#include "../utils/squares_for_prime_power.h"

/* Background:  A000224(m) = number of squares in Z/mZ
   A000010(m) = φ(m), the number of units.  We test whether
   A000224(m) >= φ(m).  The function below also computes the
   first n terms of that sequence, memoised for arbitrary
   n in any order of calls.  All arithmetic is done with the
   seq_t type supplied by the build. */

/* ---------- factorisation and auxiliary helpers ---------- */

/* test whether m satisfies the condition */
static bool is_good(uint64_t m)
{
    if (m == 1) return true;

    uint64_t n = m;
    seq_t phi = (seq_t)m;
    seq_t sq  = (seq_t)1;

    /* factorisation by trial division */
    for (uint64_t p = 2; p * p <= n; p += (p == 2 ? 1 : 2)) {
        if (n % p == 0) {
            unsigned int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            phi = phi / (seq_t)p * (seq_t)(p - 1);
            sq  = sq * (seq_t)squares_for_prime_power(p, exp);
        }
    }

    if (n > 1) {               /* remaining prime factor */
        uint64_t p = n;
        phi = phi / (seq_t)p * (seq_t)(p - 1);
        sq  = sq * (seq_t)squares_for_prime_power(p, 1);
    }

    return sq >= phi;
}

/* ---------- memoised sequence storage ---------- */

static seq_t  *seq_tbl = NULL;   /* array of the found terms */
static size_t  seq_len = 0;      /* number of terms stored so far   */
static size_t  seq_cap = 0;      /* capacity of seq_tbl              */
static uint64_t last_checked = 0;/* largest m examined so far     */

/* grow the array to at least n elements */
static void ensure_capacity(size_t n)
{
    if (seq_cap >= n) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < n) new_cap = n;
    seq_tbl = (seq_t *)realloc(seq_tbl, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* append one value to the sequence */
static void append_term(seq_t val)
{
    ensure_capacity(seq_len + 1);
    seq_tbl[seq_len++] = val;
}

/* ---------- public entry point ---------- */

seq_t a(idx_t n)
{
    /* n is 1‑based: a(1) is first term */
    if (n == 0) return (seq_t)0;   /* undefined, but safe */

    if (seq_tbl == NULL) seq_tbl = NULL;   /* initialise on first call */

    /* extend the sequence until we have at least n terms */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (is_good(last_checked))
            append_term((seq_t)last_checked);
    }
    return seq_tbl[n - 1];
}
