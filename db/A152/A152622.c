/*
 * A152622  Tetrahedral numbers n*(n+1)*(n+2)/6 with n, n+1 and n+2 nonprime.
 * https://oeis.org/A152622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2) / 6, where n, n+1 and n+2 are composite
 *            (non‑prime)
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Feed this file with <stdlib.h>, <stdint.h> and <stdbool.h>.
 * The algorithm uses the helper is_prime(idx_t) from the utils directory. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th tetrahedral number whose three consecutive arguments
 * are all composite (non‑prime).  The sequence is indexed starting at 1. */
seq_t a(idx_t n)
{
    /* Static storage for memoisation. */
    static idx_t *n_table = NULL;   /* the n values that produced each term   */
    static seq_t *v_table = NULL;   /* the corresponding tetrahedral numbers */
    static size_t size = 0;         /* number of terms currently stored      */
    static size_t cap  = 0;         /* allocated capacity                     */
    static idx_t next_candidate = 1;/* next n to test                         */

    /* Initialise storage on first call. */
    if (!n_table) {
        cap = 16;
        n_table = (idx_t *)malloc(cap * sizeof(idx_t));
        v_table = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!n_table || !v_table) abort();
    }

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        /* Find the next n with n, n+1 and n+2 all composite. */
        while (true) {
            if (!is_prime(next_candidate) &&
                !is_prime(next_candidate + 1) &&
                !is_prime(next_candidate + 2)) {

                /* Compute n*(n+1)*(n+2)/6 with safe integer maths. */
                seq_t a_val = (seq_t)next_candidate;
                seq_t b_val = (seq_t)(next_candidate + 1);
                seq_t c_val = (seq_t)(next_candidate + 2);

                /* Divide by 2 first (exact division). */
                if ((a_val & 1) == 0)
                    a_val >>= 1;
                else if ((b_val & 1) == 0)
                    b_val >>= 1;
                else
                    c_val >>= 1;

                /* Divide by 3 next. */
                if (a_val % 3 == 0)
                    a_val /= 3;
                else if (b_val % 3 == 0)
                    b_val /= 3;
                else
                    c_val /= 3;

                /* Multiply remaining factors. */
                a_val *= b_val;
                a_val *= c_val;

                /* Store the term. */
                if (size == cap) {
                    cap <<= 1;
                    n_table = (idx_t *)realloc(n_table, cap * sizeof(idx_t));
                    v_table = (seq_t *)realloc(v_table, cap * sizeof(seq_t));
                    if (!n_table || !v_table) abort();
                }
                n_table[size] = next_candidate;
                v_table[size] = a_val;
                ++size;
                ++next_candidate;   /* next candidate starts after this n   */
                break;
            }
            ++next_candidate;
        }
    }

    return v_table[n - 1];
}
