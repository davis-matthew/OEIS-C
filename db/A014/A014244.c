/*
 * A014244  (n-th Lucas number that is not 1) - (n-th number that is 1 or not
 *          a Lucas number).
 * https://oeis.org/A014244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L_{n+1} - B_{n-2} where L_k is the kth Lucas number (L_0
 *            = 2, L_1 = 1, L_k = L_{k-1} + L_{k-2}), and B_m is the m‑th
 *            positive integer that is **not** a Lucas number (B_1 = 5, B_2 =
 *            6, B_3 = 8, …). For n = 1,2,3 the sequence is defined as 2
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   is_lucas_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_lucas_uint.h"

/* The required sequence function.  */
seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly.  */
    if (n <= 3)
        return (seq_t)2;

    /* Compute the (n+1)-th Lucas number.  */
    seq_t prev2 = (seq_t)2;   /* L_0 */
    seq_t prev1 = (seq_t)1;   /* L_1 */
    seq_t lucas = 0;
    for (idx_t i = 2; i <= n + 1; ++i) {
        lucas = prev2 + prev1;
        prev2 = prev1;
        prev1 = lucas;
    }

    /* Find the (n-2)-th non‑Lucas positive integer.  */
    idx_t target = n - 2;     /* 1‑based index of the non‑Lucas sequence */
    idx_t count  = 0;
    seq_t nonLucas = 0;
    unsigned long long x = 5; /* smallest positive integer that is not a Lucas number */
    while (count < target) {
        if (!is_lucas_uint(x)) {
            ++count;
            if (count == target)
                nonLucas = (seq_t)x;
        }
        ++x;
    }

    /* Return the difference.  */
    return lucas - nonLucas;
}
