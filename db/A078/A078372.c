/*
 * A078372  Number of squarefree integers in {n, f(n), f(f(n)), ...., 1},
 *          where f is the Collatz function defined by f(x) = x/2 if x is
 *          even; f(x) = 3x + 1 if x is odd.
 * https://oeis.org/A078372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of square‑free terms in the Collatz trajectory
 *            starting at n
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Simple trial‑division test for square‑freeness.  Works for all
   values that fit into a uint64_t, which is sufficient for the
   required range (n ≤ 10 000 and the Collatz sequence stays
   well below 64 bits for these inputs). */
static bool is_squarefree_uint64(uint64_t v)
{
    for (uint64_t p = 2; p * p <= v; ++p) {
        int exp = 0;
        while (v % p == 0) {
            v /= p;
            ++exp;
            if (exp > 1)   /* p² divides v → not square‑free */
                return false;
        }
    }
    return true;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    seq_t x   = (seq_t)n;

    for (;;) {
        if (is_squarefree_uint64((uint64_t)x))
            ++cnt;

        if (x == (seq_t)1)
            break;

        /* Collatz step: if even, halve; otherwise 3x + 1 */
        if ((x & (seq_t)1) == 0)
            x = x / 2;
        else
            x = x * 3 + 1;
    }

    return cnt;
}
