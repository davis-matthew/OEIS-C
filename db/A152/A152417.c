/*
 * A152417  a(n) = (5^n - 1)/(2^(3 - (n mod 2))).
 * https://oeis.org/A152417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n - 1)/(2^(3 - (n mod 2)))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
 *   a(0) = 0, a(1) = 1
 *   for even n: a(n) = 25*a(n-2) + 3
 *   for odd  n: a(n) = 10*a(n-1) + 1
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev_even = 0;   /* a(n-2) when computing an even term          */
    seq_t current_even = 0;/* a(n-1) when computing an odd term           */
    seq_t result = 0;      /* value to return for n                         */

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1ULL) == 0) {          /* even i */
            seq_t val = 25 * prev_even + 3;
            prev_even = val;            /* now holds a(i-2) for next even          */
            current_even = val;         /* a(i) for use by following odd term      */
            if (i == n) result = val;
        } else {                         /* odd i */
            seq_t val = 10 * current_even + 1;
            if (i == n) result = val;
        }
    }
    return result;
}
