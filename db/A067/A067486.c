/*
 * A067486  Powers of 8 with initial digit 8.
 * https://oeis.org/A067486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th power of 8 whose decimal representation begins
 *            with the digit 8. The terms are ordered by increasing exponent
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   has_leading_8, power_of_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/has_leading_8.h"
#include "../utils/power_of_8.h"

#define MAX_N 20000          /* large enough for n up to about 10000 */

/* Storage for the exponents k of the terms a(n).  expo[1] = 1 for a(1)=8. */
static idx_t expo[MAX_N + 1] = {0};
static idx_t lastComputedN = 0;   /* largest n for which expo[n] is known */
static idx_t nextExp = 2;         /* next exponent to test (k=2 initially) */

/* Main sequence function.  It lazily precomputes exponents until the
   requested index n is reached. */
seq_t a(idx_t n)
{
    if (n == 0)          /* not part of the sequence (offset 1) */
        return 0;

    /* Initialise on first call */
    if (lastComputedN == 0) {
        expo[1] = 1;          /* 8^1 = 8 has leading digit 8 */
        lastComputedN = 1;
    }

    /* Extend the precomputed list up to required n.  */
    while (lastComputedN < n) {
        if (has_leading_8(nextExp)) {
            lastComputedN++;
            if (lastComputedN <= MAX_N)
                expo[lastComputedN] = nextExp;
        }
        nextExp++;
    }

    idx_t k = expo[n];
    return power_of_8(k);
}
