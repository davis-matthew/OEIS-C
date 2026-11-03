/*
 * A235507  Harshad numbers which when divided by sum of their digits, give a
 *          quotient which is a Harshad number.
 * https://oeis.org/A235507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1323, u32 n <= 10796, u64 n <= 10796,
 *            u128 n <= 10796, big any n
 * Validated: standard, pass, 77 terms through n = 9920 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Harshad numbers whose quotient by the sum of digits is also a Harshad number */

/* Computes the n‑th MHN‑2 (multiple Harshad number of order 2).  The sequence starts
   at a(1)=1.  For n==0 the function returns 0 as a safe sentinel. */
#include <stdlib.h>   /* malloc, realloc, free */
#include "../utils/mem.h"

static inline seq_t digit_sum(seq_t x) {
    seq_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

static void ensure_capacity(size_t need, seq_t **arr, size_t *cap) {
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0)   /* safety sentinel; sequence is 1‑indexed */
        return 0;

    static seq_t *list = NULL;   /* memoised sequence values    */
    static size_t cap   = 0;      /* current allocated capacity  */
    static size_t count = 0;      /* number of values stored      */
    static seq_t cand   = 1;      /* next candidate to test       */

    while (count < n) {
        seq_t sum = digit_sum(cand);
        if (sum != 0 && cand % sum == 0) {          /* cand is Harshad          */
            seq_t q   = cand / sum;                 /* the quotient              */
            seq_t qsum = digit_sum(q);
            if (qsum != 0 && q % qsum == 0) {       /* quotient is also Harshad */
                ensure_capacity(count + 1, &list, &cap);
                list[count] = cand;
                ++count;
            }
        }
        ++cand;
    }
    return list[n - 1];
}
