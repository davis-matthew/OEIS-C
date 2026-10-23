/*
 * A175898  Expansion of (1+3*x+9*x^2+9*x^3+9*x^4+3*x^5+x^6) /( (1+x)^2 *
 *          (1-x)^5 ).
 * https://oeis.org/A175898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 5*a(n-3) + 5*a(n-4) + a(n-5) -
 *            3*a(n-6) + a(n-7); Initial values:
 *            a(0)=1,a(1)=6,a(2)=26,a(3)=76,a(4)=186,a(5)=386,a(6)=726
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 328, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed, but not used here */

seq_t a(idx_t n)
{
    /* Use only the first 7 values for the recurrence. */
    static const seq_t init[7] = { 1, 6, 26, 76, 186, 386, 726 };
    if (n < 7) return init[(size_t)n];

    /* Buffer holding the last 7 computed terms. */
    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i) buf[i] = init[i];

    /* Compute successive terms up to n. */
    for (idx_t k = 7; k <= n; ++k) {
        seq_t a1 = buf[(k - 1) % 7];
        seq_t a2 = buf[(k - 2) % 7];
        seq_t a3 = buf[(k - 3) % 7];
        seq_t a4 = buf[(k - 4) % 7];
        seq_t a5 = buf[(k - 5) % 7];
        seq_t a6 = buf[(k - 6) % 7];
        seq_t a7 = buf[(k - 7) % 7];

        /* Evaluate the recurrence in a sequence that keeps intermediate
           values non‑negative for all n >= 7.        */
        seq_t val = 3 * a1;
        val -= a2;
        val -= 5 * a3;
        val += 5 * a4;
        val += a5;
        val -= 3 * a6;
        val += a7;

        buf[k % 7] = val;
    }
    return buf[n % 7];
}
