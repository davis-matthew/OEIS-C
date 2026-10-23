/*
 * A014892  n is equal to the number of 4s in all numbers <= n written in base
 *          6.
 * https://oeis.org/A014892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = n such that the number of 4's in base‑6 representations
 *            of all integers <= a(n) equals a(n). The function returns the
 *            n‑th such integer
 * Range:     u8 none, u16 n <= 7, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

#define MAX_SOL 20000   /* maximum number of solutions guaranteed */

seq_t a(idx_t n)
{
    /* static state keeps the computation across calls */
    static idx_t upto = 0;                 /* largest number processed   */
    static seq_t cum = 0;                  /* cumulative count of 4's   */
    static size_t sol_cnt = 0;             /* number of solutions known */
    static seq_t sols[MAX_SOL];            /* cache of solutions        */

/* If the requested index is larger than our buffer the function will
   continue computing until it can supply the term. The buffer is kept
   large enough for all tests in the exercise. */
    while (sol_cnt < (size_t)n) {
        ++upto;                            /* next number to examine */

        /* count digit 4 in the base‑6 representation of 'upto' */
        uint64_t t = upto;
        uint32_t add = 0;
        while (t) {
            if ((t % 6ULL) == 4ULL) ++add;
            t /= 6ULL;
        }
        cum += add;                        /* update cumulative count */

        /* a fixed point: count equals the number itself */
        if (cum == (seq_t)upto) {
            if (sol_cnt < MAX_SOL) {
                sols[sol_cnt++] = upto;
            }
        }
    }

    return sols[n - 1];
}
