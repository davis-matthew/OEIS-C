/*
 * A223735  Positive numbers that are not representable as a primitive sum of
 *          three nonzero squares.
 * https://oeis.org/A223735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, gcd, grow_seq_array, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A223735 – Positive numbers NOT representable as a primitive sum of three nonzero squares.
   The function returns the n‑th term (1‑based). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"         /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/isqrt_seq.h"    /* seq_t isqrt_seq(seq_t x) */
#include "../utils/alloc_seq_array.h" /* seq_t *alloc_seq_array(size_t n) */
#include "../utils/grow_seq_array.h"   /* void grow_seq_array(seq_t **arr, size_t *size, size_t new_len) */

/* Test whether a positive integer n can be written as a^2 + b^2 + c^2
   with 0 < a <= b <= c and gcd(a,b,c) = 1.  Return true if such a
   primitive representation exists, otherwise false.  */
static bool has_primitive_rep(seq_t n)
{
    if (n < 3)   /* need all three squares positive, minimal sum 3 */
        return false;

    /* maximum possible a: a^2 <= n/3  (since a <= b <= c) */
    seq_t a_sq_limit = n / 3;
    seq_t a_max = isqrt_seq(a_sq_limit);

    for (idx_t a = 1; a <= (idx_t)a_max; ++a) {
        seq_t a_sq = (seq_t)a * (seq_t)a;

        for (idx_t b = a; ; ++b) {
            seq_t b_sq = (seq_t)b * (seq_t)b;

            /* minimal sum with this a,b is a^2 + 2*b^2 (c >= b) */
            if (a_sq + 2 * b_sq > n)
                break;            /* no larger b will work for this a */

            seq_t rem = n - a_sq - b_sq;   /* candidate for c^2 */
            if (rem < b_sq)                /* then c < b, violates ordering */
                continue;

            seq_t c = isqrt_seq(rem);
            if (c * c == rem && c >= b) {   /* perfect square and order satisfied */
                seq_t g1 = gcd((seq_t)a, (seq_t)b);
                seq_t g  = gcd(g1, c);
                if (g == 1)
                    return true;          /* primitive triple found */
            }
        }
    }
    return false;   /* no primitive representation */
}

seq_t a(idx_t n)
{
    /* Static cache to store computed terms.  The array grows automatically. */
    static seq_t *seq = NULL;
    static size_t len = 0;          /* number of terms stored */
    static size_t cap = 0;          /* current capacity */
    static seq_t candidate = 1;     /* next integer to test */

    if (!seq) {
        cap = 1024;
        seq = alloc_seq_array(cap);
    }

    while (len < n) {
        if (!has_primitive_rep(candidate))
            /* store term if found */
            seq[len++] = candidate;

        ++candidate;
    }

    return seq[n - 1];               /* 1‑based index */
}
