/*
 * A120173  a(n) = 4 + floor((3 + Sum_{j=1..n-1} a(j))/5).
 * https://oeis.org/A120173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 + floor((3 + Sum_{j=1..n-1} a(j))/5)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 114, u64 n <= 236,
 *            u128 n <= 478, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset 1, defined for n>=1 */

    /* 64‑bit (or smaller) seq_t: use a wider sum to avoid overflow */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        typedef unsigned __int128 sum_t;
        sum_t   sum  = (sum_t)4;           /* Σ a(j) for j < current i, starts with a(1) */
        seq_t   next = (seq_t)4;           /* a(1) */

        if (n == 1) return next;

        for (idx_t i = 2; i <= n; ++i) {
            /* next = 4 + floor((sum + 3)/5) */
            sum_t tmp = (sum + (sum_t)3) / (sum_t)5;
            next = (seq_t)4 + (seq_t)tmp;
            sum += (sum_t)next;
        }
        return next;
    } else { /* seq_t is large (arbitrary‑precision) */
        seq_t sum  = (seq_t)4;
        seq_t next = (seq_t)4;

        if (n == 1) return next;

        for (idx_t i = 2; i <= n; ++i) {
            seq_t tmp = (sum + (seq_t)3) / (seq_t)5;
            next = (seq_t)4 + tmp;
            sum += next;
        }
        return next;
    }
}
