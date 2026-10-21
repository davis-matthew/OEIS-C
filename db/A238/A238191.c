/*
 * A238191  Digital root of the number of partitions of n.
 * https://oeis.org/A238191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A010888(A000041(n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   digital root of the partition number p(n)            */
/*   implementation: keep the partition number modulo 9   */
/*   then convert 0 → 9 (the only non‑zero digital root)   */
/*   dynamic programming using Euler’s pentagonal          */
/*   number recurrence (O(n√n) which is < 2 s for n≈10 000) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* mod9[i] = p(i) mod 9, stored for i = 0..current     */
    static unsigned char *mod9 = NULL;
    static idx_t cap   = 0;       /* current allocated size   */
    static idx_t len   = 0;       /* number of computed values */

    /* enlarge storage if necessary */
    if (cap < n + 1) {
        unsigned char *tmp = (unsigned char *)realloc(mod9, (n + 1) * sizeof(unsigned char));
        if (!tmp) return (seq_t)0;   /* allocation failure → 0 as a safe value */
        mod9 = tmp;
        cap  = n + 1;
    }

    /* initialise base case when first called */
    if (len == 0) {
        mod9[0] = 1;       /* p(0) = 1, 1 mod 9 */
        len = 1;
    }

    /* compute p(k) mod 9 for k = len .. n using Euler's recurrence */
    for (idx_t i = len; i <= n; ++i) {
        int v = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;

            int sign = (k & 1) ? 1 : -1;      /* (-1)^(k+1) */
            v += sign * (int)mod9[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                v += sign * (int)mod9[i - pent2];
            }
        }
        v %= 9;
        if (v < 0) v += 9;
        mod9[i] = (unsigned char)v;
    }
    len = n + 1;

    /* convert modulo 9 value to digital root */
    unsigned char root = mod9[n];
    return (seq_t)(root == 0 ? 9 : root);
}
