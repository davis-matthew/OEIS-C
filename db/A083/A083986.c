/*
 * A083986  If k is a number with exactly two distinct decimal digits, say a
 *          and b, neither of which is 0 (i.e., a member of A101594), define
 *          the self-complement of k, SC(k), to be the number obtained by
 *          replacing a with b and vice versa. Then a(n) = lcm(A101594(n),
 *          SC(A101594(n))).
 * https://oeis.org/A083986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = lcm(A101594(n), SC(A101594(n)))
 * Range:     u8 n <= 1, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A101594(n)  = the nth positive integer with exactly two distinct non‑zero decimal digits  
 *   SC(k)        = the number obtained from k by swapping those two digits
 *   a(n)         = lcm(A101594(n), SC(A101594(n)))                            */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* If true, the two digits are returned in *d1 and *d2 (order arbitrary).    */
static bool two_distinct_nonzero(uint64_t x, int *d1, int *d2)
{
    bool seen[10] = {false};
    int cnt = 0;
    int a = 0, b = 0;

    while (x != 0) {
        int d = (int)(x % 10);
        if (d == 0)
            return false;                /* zero digit not allowed */
        if (!seen[d]) {
            seen[d] = true;
            ++cnt;
            if (cnt == 1)
                a = d;
            else if (cnt == 2)
                b = d;
            else
                return false;            /* more than two distinct digits */
        }
        x /= 10;
    }

    if (cnt != 2)
        return false;

    if (d1) *d1 = a;
    if (d2) *d2 = b;
    return true;
}

/* Self‑complement: replace each occurrence of d1 by d2 and vice versa.      */
static seq_t self_complement(seq_t k)
{
    int d1 = 0, d2 = 0;
    if (!two_distinct_nonzero((uint64_t)k, &d1, &d2))
        return k;          /* should never happen */

    seq_t result = 0;
    seq_t place  = 1;
    uint64_t x   = (uint64_t)k;

    while (x != 0) {
        int d  = (int)(x % 10);
        int nd = (d == d1) ? d2 : d1;
        result += (seq_t)nd * place;
        place  *= (seq_t)10;
        x      /= 10;
    }
    return result;
}

/* ------------------------------------------------------------------------- */
/* Simple Euclidean GCD that works for any unsigned integer type            */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------------- */
/* Test whether x has exactly two distinct non‑zero decimal digits           */

/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Cache for A101594: dynamic array storing the generated terms.  */
static seq_t *A101594_cache   = NULL;
static size_t  A101594_size   = 0;
static size_t  A101594_capacity = 0;
static uint64_t A101594_next  = 12;   /* first candidate with two digits */

/* Retrieve the nth term of A101594 (1‑based).  */
static seq_t get_A101594(idx_t n)
{
    while (A101594_size < (size_t)n) {
        /* test current candidate */
        int d1, d2;
        if (two_distinct_nonzero(A101594_next, &d1, &d2)) {
            /* store it */
            if (A101594_size == A101594_capacity) {
                size_t new_cap = (A101594_capacity == 0) ? 32 : A101594_capacity * 2;
                seq_t *tmp = mem_resize(seq_t, A101594_cache, new_cap);
                if (!tmp)
                    abort();   /* memory allocation failed */
                A101594_cache   = tmp;
                A101594_capacity = new_cap;
            }
            A101594_cache[A101594_size++] = (seq_t)A101594_next;
        }
        ++A101594_next;
    }
    return A101594_cache[(size_t)n - 1];
}

/* ------------------------------------------------------------------------- */
/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    seq_t k  = get_A101594(n);
    seq_t sc = self_complement(k);
    seq_t g  = seq_gcd(k, sc);
    seq_t lcm = (k / g) * sc;    /* lcm = k * sc / gcd(k,sc) */
    return lcm;
}
