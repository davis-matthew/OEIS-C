/*
 * A101884  Smallest increasing natural number sequence without any length 3
 *          equidistant arithmetic subsequences.
 * https://oeis.org/A101884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth smallest natural number such that for all *
 *            indices 1 ≤ i < j < k ≤ n, if k - i = j - i then * a(i), a(j),
 *            a(k) are not an arithmetic progression. * The sequence is
 *            constructed greedily: each term is the smallest * integer larger
 *            than the previous term that does not create * a len...
 * Range:     u8 n <= 91, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static void ensure_capacity(idx_t needed)
{
    static size_t cap = 0;
    static seq_t *seq = NULL;

    if ((size_t)needed <= cap)
        return;
    size_t newcap = cap ? cap * 2 : 16;
    while ((size_t)needed > newcap)
        newcap *= 2;
    seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    cap = newcap;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static idx_t size = 0;

    if (!seq) {
        /* first call: initialise storage */
        seq = (seq_t *)malloc(16 * sizeof(seq_t));
        cap = 16;
    }

    if ((size_t)n > cap) {
        size_t newcap = cap;
        while ((size_t)n > newcap)
            newcap *= 2;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        cap = newcap;
    }

    while (size < n) {
        if (size == 0) {
            seq[0] = (seq_t)1;
            ++size;
            continue;
        }

        seq_t cand = seq[size - 1] + (seq_t)1;
        idx_t new_idx = size + 1; /* 1‑based index of the new term */

        while (1) {
            bool ok = true;

            /* check all i such that (i + new_idx) is even */
            for (idx_t i = 1; i <= size; ++i) {
                if (((i + new_idx) & 1) == 0) {      /* j is integer */
                    idx_t j = (i + new_idx) >> 1;   /* 1‑based index of middle term */
                    if (j <= size) {
                        seq_t diff1 = seq[j - 1] - seq[i - 1];
                        seq_t diff2 = cand - seq[j - 1];
                        if (diff1 == diff2) {
                            ok = false;
                            break;
                        }
                    }
                }
            }

            if (ok) {
                seq[size] = cand;
                ++size;
                break;
            }
            ++cand; /* try next integer */
        }
    }

    return seq[n - 1];
}
