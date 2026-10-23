/*
 * A183409  Number of n X 2 binary arrays with each sum of a(1..i,1..j) no
 *          greater than i*j/2 and rows and columns in nondecreasing order.
 * https://oeis.org/A183409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^3 + 11*n^2 + 31*n + 27 + (n^2 + n + 5)*(-1)^n) / 32
 * Range:     u8 n <= 13, u16 n <= 99, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset starts at 1 */

    unsigned long long N = (unsigned long long)n;

    /* Compute numerator using signed 64‑bit arithmetic (safe for n ≤ 10⁴) */
    signed long long base   = 2LL * N * N * N
                            + 11LL * N * N
                            + 31LL * N
                            + 27LL;
    signed long long parity = (signed long long)(N * N + N + 5ULL);

    signed long long num = ((n & 1ULL) ? base - parity : base + parity);

    return (seq_t)(num / 32LL);
}
