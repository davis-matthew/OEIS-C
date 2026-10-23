/*
 * A105261  Values of n such that phi(n)=c(n)^2, where phi is the Euler
 *          totient function and c(n) is the product of the distinct prime
 *          factors of n (c(1)=1).
 * https://oeis.org/A105261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) is the nth integer n such that phi(n) = rad(n)^2
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   build_phi_rad, find_solutions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_phi_rad.h"
#include "../utils/find_solutions.h"

/* Public function */
seq_t a(idx_t n)
{
    /* static data for memoisation */
    static bool init = false;
    static idx_t count = 0;
    static seq_t *values = NULL;

    if (!init) {
        const idx_t MAX_N = 200000;  /* larger than largest known term 41154 */
        find_solutions(MAX_N, &count, &values);
        init = true;
    }
    if (n == 0 || n > count) {
        /* Undefined for n beyond known terms: return 0 */
        return (seq_t)0;
    }
    return values[n - 1];
}
