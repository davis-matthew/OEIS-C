/*
 * A109546  a(n) = (4^(n+1)-(-1)^n 9 )/5.
 * https://oeis.org/A109546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A109546:  a(n) = (4^(n+1) - (-1)^n * 9) / 5   (offset 1)
    Recurrence used for n ≤ 10000:
        a(1) = 5
        a(n) = 4*a(n-1) + ((n % 2 == 1) ? 9 : -9)   for n ≥ 2
    For n > 10000 the closed form is evaluated directly. */

seq_t a(idx_t n)
{
    if (n == 0) return -1;                /* undefined offset 0, kept for completeness */

    /* --- memoised recurrence for n ≤ 10000 -------------------------------- */
    if (n <= 10000) {
        static seq_t memo[10001];          /* indices 0..10000, we keep memo[1]..memo[10000]   */
        static idx_t computed = 0;         /* largest index computed so far */

        if (computed == 0) {              /* initialise with a(1) */
            memo[1] = 5;
            computed = 1;
        }

        if (n > computed) {
            for (idx_t i = computed + 1; i <= n; ++i) {
                seq_t prev = memo[i - 1];
                if (i & 1)                /* i odd: +9 */
                    memo[i] = prev * 4 + 9;
                else                      /* i even: -9 */
                    memo[i] = prev * 4 - 9;
            }
            computed = n;
        }
        return memo[n];
    }

    /* --- closed form for n > 10000 ----------------------------------- */
    seq_t power4 = 1;
    for (idx_t i = 0; i < n + 1; ++i)
        power4 *= 4;

    seq_t term = (n & 1) ? power4 + 9 : power4 - 9;   /* term = 4^(n+1) - (-1)^n*9 */
    return term / 5;
}
