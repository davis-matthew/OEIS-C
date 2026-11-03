/*
 * A125469  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) follows the linear recurrence * a(n) = 12*a(n-1) - 3*a(n-2)
 *            - 26*a(n-3) + a(n-4) + 8*a(n-5) * for all n ≥ 6, with initial
 *            values * a(0)=1, a(1)=12, a(2)=138, a(3)=1542, a(4)=17782. *
 *            a(5) is obtained by a short brute‑force enumeration of 5‑digit *
 *            circular base‑12 sequences (248832 possibili...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: absolute difference of two ints */
static inline int absdiffint(int a, int b)
{
    return a >= b ? a - b : b - a;
}

/* Compute a(5) by brute force enumeration */
static seq_t compute_a5(void)
{
    seq_t cnt = 0;
    for (int d0 = 0; d0 < 12; ++d0) {
        for (int d1 = 0; d1 < 12; ++d1) {
            if (absdiffint(d0, d1) > 9) continue;
            for (int d2 = 0; d2 < 12; ++d2) {
                if (absdiffint(d1, d2) > 9) continue;
                for (int d3 = 0; d3 < 12; ++d3) {
                    if (absdiffint(d2, d3) > 9) continue;
                    for (int d4 = 0; d4 < 12; ++d4) {
                        if (absdiffint(d3, d4) > 9) continue;
                        if (absdiffint(d4, d0) > 9) continue;
                        ++cnt;
                    }
                }
            }
        }
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return 1;
    if (n == 1) return 12;
    if (n == 2) return 138;
    if (n == 3) return 1542;
    if (n == 4) return 17782;

    /* Compute and cache a(5) once */
    static seq_t a5_val;
    static bool a5_computed = false;
    if (!a5_computed) {
        a5_val = compute_a5();        /* should equal 205222 */
        a5_computed = true;
    }
    if (n == 5) return a5_val;

    /* Compute a(n) for n >= 6 using the recurrence */
    seq_t prev5 = 12;          /* a(1) */
    seq_t prev4 = 138;         /* a(2) */
    seq_t prev3 = 1542;        /* a(3) */
    seq_t prev2 = 17782;       /* a(4) */
    seq_t prev1 = a5_val;      /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = ((seq_t)12 * prev1)
                   - ((seq_t)3  * prev2)
                   - ((seq_t)26 * prev3)
                   + prev4
                   + ((seq_t)8  * prev5);

        /* shift the window */
        prev5 = prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
