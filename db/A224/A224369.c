/*
 * A224369  Number of n X 3 0..2 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 16*a(n-2) + 30*a(n-3) - 14*a(n-4) + 12*a(n-5)
 *            (for n ≥ 6; a(1)=22, a(2)=218, a(3)=1838, a(4)=15540,
 *            a(5)=132236)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; below offset is undefined */
    if (n == 1) return 22;
    if (n == 2) return 218;
    if (n == 3) return 1838;
    if (n == 4) return 15540;
    if (n == 5) return 132236;

    /* a(1) … a(5) are the initial terms */
    seq_t r0 = 22;     /* a(n-5) */
    seq_t r1 = 218;    /* a(n-4) */
    seq_t r2 = 1838;   /* a(n-3) */
    seq_t r3 = 15540;  /* a(n-2) */
    seq_t r4 = 132236; /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i)
    {
        /* compute a(i) = 10*a(i-1) - 16*a(i-2) + 30*a(i-3) - 14*a(i-4) + 12*a(i-5) */
        seq_t sum_pos = ((seq_t)10) * r4 + ((seq_t)30) * r2 + ((seq_t)12) * r0;
        seq_t sum_neg = ((seq_t)16) * r3 + ((seq_t)14) * r1;
        seq_t next = sum_pos - sum_neg;  /* first sum is always ≥ second sum */

        /* shift history: drop oldest (r0) and append next */
        r0 = r1;
        r1 = r2;
        r2 = r3;
        r3 = r4;
        r4 = next;
    }
    return r4;  /* r4 now holds a(n) */
}
