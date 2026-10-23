/*
 * A168820  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   For n ≥ 1:  a(n) = p(n) + 45·a(n‑1) + 45·a(n‑2) + … + 45·a(n‑18)   if n ≤ 18
               a(19) = 1 + 45·(a(18)+…+a(1)) – 1035·a(0)
   For n ≥ 20: a(n) = 45·(a(n‑1)+…+a(n‑18)) – 1035·a(n‑19)
   where p(n)=2 for 1 ≤ n ≤ 18, p(19)=1, p(0)=1, p(n)=0 otherwise.
 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static idx_t max_comp = 0;
    static size_t cap = 0;

    /* Initial allocation: enough space for at least 20 entries */
    if (arr == NULL) {
        cap = (n > 19 ? n + 1 : 20);
        arr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!arr) abort();                /* out of memory */
        arr[0] = (seq_t)1;                /* a(0) = 1 */
        max_comp = 0;
    }

    /* Grow the array if needed */
    if (n >= cap) {
        size_t new_cap = (size_t)(n + 1) * 2;   /* double strategy */
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        arr = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;                /* a(0) already set */
        } else if (i <= 18) {
            val = (seq_t)2;                /* p(i) = 2 for 1 ≤ i ≤ 18 */
        } else if (i == 19) {
            val = (seq_t)1;                /* p(19) = 1 */
        } else {
            val = (seq_t)0;                /* p(i) = 0 for i ≥ 20 */
        }

        /* Add contributions: 45·a(i‑k) for k = 1…18, and –1035·a(i‑19) for k = 19 */
        idx_t limit = (i <= 19 ? i : 19);
        for (idx_t k = 1; k <= limit; ++k) {
            seq_t term = arr[i - k];
            if (k == 19) {
                val -= (seq_t)1035 * term;   /* subtract 1035·a(i‑19) */
            } else {
                val += (seq_t)45 * term;     /* add 45·a(i‑k) */
            }
        }
        arr[i] = val;
    }

    max_comp = n;
    return arr[n];
}
