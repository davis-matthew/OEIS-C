/*
 * A184482  Lower s-Wythoff sequence, where s(n)=3n-1. Complement of A184483.
 * https://oeis.org/A184482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition:
   Let s(n) = 3n-1.
   The lower s-Wythoff sequence a(n) is defined by
       a(1) = 1,
   and for n > 1
       a(n) is the least natural number not equal to any
       a(k) or a(k)+s(k) for 1 ≤ k < n.
   Equivalently, a(n) is the mex of all previously used numbers,
   where each step adds the pair (a(n), a(n)+3n-1). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t max_n = 0;            /* largest n computed so far */
    static seq_t *seq = NULL;          /* array of a-values */
    static bool *used = NULL;          /* marks numbers already used */
    static size_t used_cap = 0;        /* capacity of used[] */
    static size_t seq_cap = 0;         /* capacity of seq[] */
    static idx_t next_unused = 1;      /* smallest candidate number */

    if (n == 0) return 0;             /* no meaning for offset 0 */

    /* ensure seq array can hold n elements */
    if (seq_cap < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(seq, (size_t)n * sizeof(seq_t));
        /* in a competition environment we ignore allocation failure */
        seq = tmp;
        seq_cap = (size_t)n;
    }

    /* estimate maximum value that will be needed in used[] */
    /* b(n) = a(n) + 3n-1 ≈ (4/3)n + 3n = 13n/3, so use 13n/3 + 10 */
    idx_t est_max_val = (13 * n) / 3 + 10;
    if (used_cap < (size_t)est_max_val + 1) {
        size_t new_cap = (size_t)est_max_val + 1;
        bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
        /* initialise newly added part to false */
        for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
        used = tmp;
        used_cap = new_cap;
    }

    /* generate new terms up to n if necessary */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* find next unused number */
        while (next_unused < (idx_t)used_cap && used[(size_t)next_unused])
            ++next_unused;

        /* if next_unused exceeds current capacity, grow the array */
        if ((idx_t)next_unused >= (idx_t)used_cap) {
            size_t new_cap = used_cap == 0 ? 64 : used_cap * 2;
            bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
            for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
            used = tmp;
            used_cap = new_cap;
        }

        seq_t a_k = (seq_t)next_unused;
        seq[k - 1] = a_k;

        /* compute upper partner */
        seq_t b_k = a_k + ((seq_t)3 * k - 1);

        /* ensure used[] can hold b_k */
        if ((size_t)b_k >= used_cap) {
            size_t new_cap = (size_t)b_k + 64;
            bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
            for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
            used = tmp;
            used_cap = new_cap;
        }

        used[(size_t)a_k] = true;
        used[(size_t)b_k] = true;
    }

    max_n = n;
    return seq[n - 1];
}
