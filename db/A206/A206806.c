/*
 * A206806  Sum_{0<j<k<=n} s(k)-s(j), where s(j)=A002620(j) is the j-th
 *          quarter-square.
 * https://oeis.org/A206806
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1+3*(-1)^n-2*n+2*n^2+2*n^3)/48
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 566, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n < 2 the sequence is undefined, but return 0 safely */
    if (n < 2)
        return 0;

    /* use signed 128‑bit arithmetic for intermediate results */
    const __int128 N     = n;
    const __int128 term1 = (n % 2 == 0) ? 4 : -2;          /* 1+3*(-1)^n  */
    const __int128 expr  = term1 - 2 * N + 2 * N * N + 2 * N * N * N;
    const __int128 numer = N * expr;
    const seq_t result   = (seq_t)(numer / 48);            /* exact division */
    return result;
}
