/*
 * A022802  a(n) = L(n+1) + c(n) where L(k) = k-th Lucas number and c(n) is
 *          n-th number that is 1 or not a Lucas number.
 * https://oeis.org/A022802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(n+1) + c(n) where L(k) is the k‑th Lucas number
 *            (L(0)=2, L(1)=1, L(k)=L(k‑1)+L(k‑2)) and c(n) is the n‑th
 *            positive integer that is 1 or not a Lucas number
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_lucas_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_lucas_small.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* 0‑based array: lucas[i] holds L(i).  */
    static seq_t *lucas = NULL;
    static idx_t lucas_len = 0;
    static idx_t lucas_cap = 0;

    /* Initialise the cache on first use. */
    if (lucas_len == 0) {
        lucas_cap = 16;
        lucas = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        if (!lucas) return (seq_t)0;          /* out‑of‑memory guard */
        lucas[0] = (seq_t)2;   /* L(0) */
        lucas[1] = (seq_t)1;   /* L(1) */
        lucas_len = 2;
    }

    /* Need value L(n+1).  Resize and compute missing terms. */
    size_t desired = (size_t)(n + 2);    /* indices 0 .. n+1 inclusive */
    if (desired > lucas_cap) {
        size_t new_cap = lucas_cap;
        while (new_cap < desired)
            new_cap = new_cap * 2;
        seq_t *tmp = (seq_t *)realloc(lucas, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory guard */
        lucas = tmp;
        lucas_cap = new_cap;
    }
    for (size_t i = lucas_len; i < desired; ++i) {
        lucas[i] = lucas[i - 1] + lucas[i - 2];
    }
    lucas_len = desired;

    seq_t lucas_val = lucas[n + 1];

    /* Compute c(n): the n‑th integer that is 1 or not a Lucas number. */
    idx_t count = 0;
    idx_t x = 1;
    idx_t c_val = 0;
    while (true) {
        if (x == 1 || !is_lucas_small(x)) {
            ++count;
            if (count == n) {             /* n ≥ 1, so will be found */
                c_val = x;
                break;
            }
        }
        ++x;
    }

    return lucas_val + (seq_t)c_val;
}
