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
 * Helpers:   self_complement, two_distinct_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A101594(n)  = the nth positive integer with exactly two distinct non‑zero decimal digits  
 *   SC(k)        = the number obtained from k by swapping those two digits
 *   a(n)         = lcm(A101594(n), SC(A101594(n)))                            */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/self_complement.h"
#include "../utils/two_distinct_nonzero.h"

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
                seq_t *tmp = (seq_t *)realloc(A101594_cache, new_cap * sizeof(seq_t));
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
