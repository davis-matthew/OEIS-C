/*
 * A240830  a(n)=1 for n <= s+k; thereafter a(n) =
 *          Sum(a(n-i-s-a(n-i-1)),i=0..k-1) where s=0, k=7.
 * https://oeis.org/A240830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, hear
 * Formula:   a(n) = 1 for n ≤ 7; otherwise a(n) = Σ_{i=0}^{6} a(n – i – a(n –
 *            i – 1)) (s = 0, k = 7)
 * Range:     u8 n <= 291, u16 n <= 9952, u32 n <= 9952, u64 n <= 9952,
 *            u128 n <= 9952, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out‑of‑range request, defined as 0 */

    /* static memoization storage */
    static seq_t *buf = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;

    /* ensure capacity for index n (1‑based) */
    idx_t needed = n + 1;          /* we store a[1]..a[n] */
    if ((size_t)needed > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while ((size_t)needed > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* allocation failure; unlikely in tests */
        buf = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 7) {
                buf[i] = (seq_t)1;
            } else {
                seq_t sum = 0;
                for (idx_t j = 0; j <= 6; ++j) {
                    idx_t idx = i - j - (idx_t)buf[i - j - 1];
                    sum += buf[idx];
                }
                buf[i] = sum;
            }
        }
        max_computed = n;
    }

    return buf[n];
}
