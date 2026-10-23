/*
 * A188380  Balanced ternary Keith numbers.
 * https://oeis.org/A188380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) is the n‑th positive integer N whose balanced ternary
 *            digits d1,…,dk (with d1 least significant) satisfy the k‑adic
 *            recurrence s_i = d_i (1 ≤ i ≤ k) s_i = s_{i‑k}+…+s_{i‑1} (i > k)
 *            the sequence eventually generates N
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   is_balanced_ternary_keith, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/is_balanced_ternary_keith.h"

/* Forward declaration.  */
static bool is_balanced_ternary_keith(seq_t);

/* Main sequence accessor.  */
seq_t a(idx_t n)
{
    /* Static state.  */
    static seq_t *seq = NULL;     /* array of found Keith numbers      */
    static size_t len = 0;        /* number of computed terms         */
    static size_t cap = 0;        /* allocated capacity              */
    static idx_t last_checked = 0;/* last candidate examined          */

    if (n == 0)
        return 0; /* undefined for offset 1 */

    /* Generate more terms if needed.  */
    if (len < (size_t)n) {
        size_t target = (size_t)n;
        while (len < target) {
            seq_t cand = (seq_t)(last_checked + 1);
            ++last_checked;

            /* Skip the trivial case N == 1.  */
            if (cand == (seq_t)1)
                continue;

            if (is_balanced_ternary_keith(cand)) {
                /* Grow buffer if necessary.  */
                if (len == cap) {
                    size_t new_cap = (cap == 0) ? 16 : cap * 2;
                    seq_t *tmp = seq_alloc_array(new_cap);
                    for (size_t i = 0; i < len; ++i)
                        tmp[i] = seq[i];
                    if (seq)
                        seq_free_array(seq);
                    seq = tmp;
                    cap = new_cap;
                }
                seq[len++] = cand;
            }
        }
    }
    return seq[n - 1];
}
