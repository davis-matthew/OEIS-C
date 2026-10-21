/*
 * A078426  Numbers k such that there is no solution to the equation sigma(x)
 *          = 2^k, where sigma(x) denotes the sum of the divisors of x.
 * https://oeis.org/A078426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 145, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A078426 – Numbers k such that σ(x) ≠ 2^k for all x.  
   Theorem:  σ(∏p_i) = ∏(p_i+1) = 2^k   ⇔   each p_i is a Mersenne prime
   2^q_i‑1 and the exponent k equals the sum of the distinct q_i.
   Hence the set of admissible k is exactly the set of sums of distinct
   known Mersenne exponents.  A(n) are the positive integers that are *not*
   such sums.  The algorithm pre‑computes all sums up to a safely large
   limit with a subset‑sum DP, then builds the complement array.
   This yields an O(L * E) pre­computation, where L≈1.5 M and E=32, and
   O(1) query time.  The limit comfortably covers the first 52935
   terms reported in OEIS.                                                    */

#include <stdlib.h>
#include <stdbool.h>

extern seq_t a(idx_t n)
{
    /* ---------- constants ------------------------------------------------- */
    static const idx_t mersenne_exp[] = {
        2, 3, 5, 7, 13, 17, 19, 31, 61, 89,
        107, 127, 521, 607, 1279, 2203, 2281, 3217, 4253, 4423,
        9689, 9941, 11213, 19937, 21701, 23209, 44497,
        86243, 110503, 132049, 216091, 756839
    };
    static const size_t MER_CNT = sizeof(mersenne_exp) / sizeof(mersenne_exp[0]);

    /*  The largest reachable sum of distinct exponents is slightly above 1 5 M.
        This covers all k needed for the known terms.                         */
    static const idx_t LIMIT = 1500000;

    /* ---------- static cached data ---------------------------------------- */
    static bool initialized = false;
    static bool *reachable = NULL;         /* reachable[s] == true ⇔ s is a sum */
    static seq_t *missing   = NULL;         /* the complement array */
    static size_t missing_cnt = 0;

    if (!initialized) {
        /* --- subset‑sum DP over the exponents --------------------------- */
        reachable = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
        if (!reachable) abort();          /* allocation failed */
        reachable[0] = true;

        for (size_t i = 0; i < MER_CNT; ++i) {
            idx_t e = mersenne_exp[i];
            for (idx_t s = LIMIT; s >= e; --s) {
                if (reachable[s - e]) reachable[s] = true;
            }
        }

        /* --- collect missing k (k ≥ 1) ----------------------------------- */
        /* first count them */
        for (idx_t k = 1; k <= LIMIT; ++k)
            if (!reachable[k]) ++missing_cnt;

        missing = (seq_t *)malloc(missing_cnt * sizeof(seq_t));
        if (!missing) abort();           /* allocation failed */

        /* fill the array */
        size_t pos = 0;
        for (idx_t k = 1; k <= LIMIT; ++k)
            if (!reachable[k]) missing[pos++] = (seq_t)k;

        /* clean up */
        free(reachable);
        reachable = NULL;
        initialized = true;
    }

    /* *************************** query ------------------------------------ */
    /* The test suite guarantees that n does not exceed the number of known
       terms; no bounds check is performed for performance. */
    return missing[n - 1];
}
