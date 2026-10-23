/*
 * A120177  a(1)=9; a(n)=floor((47+sum(a(1) to a(n-1)))/5).
 * https://oeis.org/A120177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(47 + Σ_{k=1}^{n-1} a(k)) / 5⌋ , a(1) = 9
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 110, u64 n <= 232,
 *            u128 n <= 472, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;          /* cache of a(1)..a(max_n)   */
    static idx_t  max_n   = 0;            /* highest n computed so far */
    static unsigned __int128 sum128 = 0;  /* cumulative sum for small seq_t */
    static seq_t       sum_seq   = 0;     /* cumulative sum for large seq_t */

    if (n == 0) return (seq_t)0;          /* out of range request – harmless */

    if (n > max_n) {
        size_t new_cap = (size_t)(n + 1);  /* indices 0..n, 0 unused */
        a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));

        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t term;
            if (sizeof(seq_t) <= 8) { /* seq_t is 8 bytes or less */
                unsigned __int128 num = (unsigned __int128)47 + sum128;
                unsigned __int128 div = num / 5;
                term = (seq_t)div;
                sum128 += div;
            } else { /* arbitrary‑precision seq_t */
                seq_t num = sum_seq + (seq_t)47;
                seq_t div = num / (seq_t)5;
                term = div;
                sum_seq += div;
            }
            a_vals[i] = term;
        }
        max_n = n;
    }
    return a_vals[n];
}
