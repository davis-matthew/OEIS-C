/*
 * A097870  Molien series for group of order 4608 acting on joint weight
 *          enumerators of a pair of binary doubly-even self-dual codes.
 * https://oeis.org/A097870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A097870 – Molien series for a group of order 4608
 *
 *  The generating function is
 *      G(x) = (1 + x^2 + 2x^3 + x^4 + x^5 + x^6 + x^7) /
 *             (1 - 2x + x^2 - 2x^3 + 4x^4 - 2x^5 + x^6 - 2x^7 + x^8).
 *
 *  Equivalently a(n) satisfies the linear recurrence
 *
 *      a(n) =  2a(n-1) - a(n-2) + 2a(n-3) - 4a(n-4) + 2a(n-5)
 *              - a(n-6) + 2a(n-7) - a(n-8)          for n ≥ 8
 *
 *  with initial values
 *
 *      a(0)=1, a(1)=2, a(2)=4, a(3)=10, a(4)=17,
 *      a(5)=27, a(6)=45, a(7)=66.
 *
 *  The function recomputes terms up to the requested index, caching
 *  all previous values so that repeated calls for arbitrary indices
 *  are fast.  The implementation uses only integer arithmetic
 *  allowed for seq_t (addition, subtraction, multiplication with
 *  small constants) and works for all supported seq_t/idx_t types.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for a(0) … a(7) */
    static const seq_t init[8] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)10,
                                   (seq_t)17, (seq_t)27, (seq_t)45, (seq_t)66 };

    /* Cached sequence and storage state */
    static seq_t *terms = NULL;   /* array holding a(0)…a(k) */
    static idx_t  used  = 0;      /* number of values currently stored */
    static idx_t  cap   = 0;      /* allocated capacity */

    /* Ensure storage can contain up to n */
    if (n >= used) {
        /* Grow capacity if necessary */
        if (cap <= n) {
            idx_t new_cap = (cap == 0) ? 16 : cap;
            while (new_cap <= n) new_cap <<= 1;
            terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
            if (!terms) return (seq_t)0;   /* allocation failed – unlikely in tests */
            cap = new_cap;
        }

        /* On first call, set known initial terms up to min(n,7) */
        if (used == 0) {
            for (idx_t i = 0; i < 8 && i <= n; ++i)
                terms[i] = init[i];
            used = (n < 8) ? n + 1 : 8;
        }

        /* Compute all missing terms up to n */
        for (idx_t i = used; i <= n; ++i) {
            if (i < 8) {                   /* missing initial value */
                terms[i] = init[i];
                continue;
            }
            /* Recurrence for n ≥ 8 */
            seq_t val = (seq_t)2 * terms[i-1];
            val -= terms[i-2];
            val += (seq_t)2 * terms[i-3];
            val -= (seq_t)4 * terms[i-4];
            val += (seq_t)2 * terms[i-5];
            val -= terms[i-6];
            val += (seq_t)2 * terms[i-7];
            val -= terms[i-8];
            terms[i] = val;
        }
        used = n + 1;
    }

    return terms[n];
}
