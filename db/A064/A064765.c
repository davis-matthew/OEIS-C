/*
 * A064765  a(n) is the smallest number such that for, all m<n, both a(n)*a(m)
 *          and a(n)+a(m) are nonsquares.
 * https://oeis.org/A064765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064765: a(n) is the smallest integer such that for all m < n,
   both a(n)+a(m) and a(n)*a(m) are nonsquares.
   The function uses a straightforward brute‑force search with memoisation. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Return true if x is a perfect square. */
static inline bool is_square(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* Compute the n-th term (n >= 1).  The function keeps a static cache
   of already‑computed values so that any order of calls is handled. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1 */
        return (seq_t)0;

    /* static cache of computed terms */
    static seq_t *seq = NULL;
    static size_t cap = 0;   /* allocated capacity in number of elems */
    static idx_t len = 0;    /* number of terms currently stored */

    /* Ensure space for n terms */
    if ((size_t)len + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap < (size_t)len + 1)
            new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Build up the sequence until the n-th term is available */
    while (len < n) {
        seq_t candidate;
        if (len == 0)               /* a(1) = 1 */
            candidate = (seq_t)1;
        else                        /* start searching from a(len) + 1 */
            candidate = seq[len - 1] + (seq_t)1;

        bool ok = false;
        while (!ok) {
            ok = true;
            for (idx_t i = 0; i < len; ++i) {
                seq_t sum = candidate + seq[i];
                if (is_square(sum)) {
                    ok = false;
                    break;
                }
                seq_t prod = candidate * seq[i];
                if (is_square(prod)) {
                    ok = false;
                    break;
                }
            }
            if (!ok)
                ++candidate;          /* try the next integer */
        }

        seq[len++] = candidate;          /* store the new term */
    }

    return seq[n - 1];
}
