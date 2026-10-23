/*
 * A198586  a(n) = (4^A001651(n+1) - 1)/3: numbers (4^k-1)/3 for k > 1, not
 *          multiples of 3.
 * https://oeis.org/A198586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 64*a(n-2) - 64*a(n-3) for n>3 with a(1)=5,
 *            a(2)=85, a(3)=341 (equivalent to a(n) = (-1 + (-8)^n + 3*8^n) /
 *            3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return 5;
    if (n == 2) return 85;
    if (n == 3) return 341;

    /* Compute via recurrence: a(n) = a(n-1) + 64*a(n-2) - 64*a(n-3) */
    seq_t prev3 = 5;     /* a(n-3) */
    seq_t prev2 = 85;    /* a(n-2) */
    seq_t prev1 = 341;   /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = prev1 + (prev2 * 64) - (prev3 * 64);
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
