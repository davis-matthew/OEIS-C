/*
 * A215739  Even and odd composite squarefree numbers, interlaced.
 * https://oeis.org/A215739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k-1) = A039956(k+1) (even squarefree numbers) a(2k) =
 *            A024556(k) (odd squarefree composite numbers)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/is_prime_small.h"

/* Ensure capacity of a dynamic array of seq_t */
static void ensure_cap(size_t needed, seq_t **arr, size_t *cap) {
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined but safe */

    /* Static cache for the two subsequences */
    static seq_t *even_sqfree   = NULL; /* even squarefree numbers */
    static size_t even_cap = 0, even_len = 0;
    static uint64_t even_next = 2;     /* next even candidate */

    static seq_t *odd_sqfree_comp  = NULL; /* odd squarefree composite numbers */
    static size_t odd_cap   = 0, odd_len = 0;
    static uint64_t odd_next  = 3;      /* next odd candidate */

    /* Determine which subsequence is needed */
    bool need_even = (n & 1); /* odd index -> even subsequence */

    if (need_even) {
        /* a(2k-1) = A039956(k+1); k = (n+1)/2; zero‑based index = k */
        idx_t k   = (n + 1) >> 1;          /* (n+1)/2 */
        size_t idx = (size_t)k;            /* zero‑based array index */

        while (even_len <= idx) {
            uint64_t cand = even_next;
            even_next += 2;
            if (is_squarefree((seq_t)cand)) {
                ensure_cap(even_len + 1, &even_sqfree, &even_cap);
                even_sqfree[even_len++] = (seq_t)cand;
            }
        }
        return even_sqfree[idx];
    } else {
        /* a(2k) = A024556(k); k = n/2; zero‑based index = k-1 */
        idx_t k   = n >> 1;                 /* n/2 */
        size_t idx = (size_t)(k - 1);       /* zero‑based array index */

        while (odd_len <= idx) {
            uint64_t cand = odd_next;
            odd_next += 2;
            if (cand > 1 &&
                !is_prime_small((seq_t)cand) &&
                is_squarefree((seq_t)cand)) {
                ensure_cap(odd_len + 1, &odd_sqfree_comp, &odd_cap);
                odd_sqfree_comp[odd_len++] = (seq_t)cand;
            }
        }
        return odd_sqfree_comp[idx];
    }
}
