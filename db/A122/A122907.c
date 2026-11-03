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
 * Validated: standard, pass, 76 terms through n = 9960 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* number of squares modulo 2^e (e>=1) */
static seq_t squares_for_2pow(unsigned int e)
{
    /* T(1)=2, T(2)=2; for e>=3, T(e)=2^(e-3)+T(e-2) */
    if (e <= 2) return (seq_t)2;
    seq_t t_prev2 = (seq_t)2;   /* T(2) */
    seq_t t_prev1 = (seq_t)3;   /* T(3) */
    seq_t cur_val;
    if (e == 2) return t_prev2;
    if (e == 3) return t_prev1;
    for (unsigned int cur = 4; cur <= e; ++cur) {
        cur_val = (((seq_t)1) << (cur - 3)) + t_prev2;  /* 2^(cur-3)+T(cur-2) */
        t_prev2 = t_prev1;
        t_prev1 = cur_val;
    }
    return t_prev1;
}

/* number of squares modulo p^e for arbitrary prime p */
static seq_t squares_for_prime_power(uint64_t p, unsigned int e)
{
    if (p == 2) return squares_for_2pow(e);
    /* squares = ((p-1)/2) * p^(e-1) + 1 */
    seq_t pow = 1;
    for (unsigned int i = 1; i < e; ++i) pow *= (seq_t)p;
    return ((seq_t)(p - 1) / 2) * pow + (seq_t)1;
}

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
    seq_tbl = mem_resize(seq_t, seq_tbl, new_cap);
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
