/*
 * A093613  Table read by rows: row n is the n-th Primary Phyllotaxis Sequence
 *          (PPS), which has F(n+1) terms, where F(n) is the n-th Fibonacci
 *          number. For 1 <= k < F(n+1), a(n, k) = -k*F(n-1) mod F(n+1). a(n,
 *          F(n+1)) = F(n+1).
 * https://oeis.org/A093613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   for each n >= 1 let row r be such that * S(r-1) < n <= S(r),
 *            where S(r) = sum_{i=1}^r Fib(i+1) * (Fib(0)=0, Fib(1)=1). Then
 *            let k = n - S(r-1). * If k == Fib(r+1) return Fib(r+1); *
 *            otherwise return -k*Fib(r-1) mod Fib(r+1)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset; behaviour below 1 is irrelevant */
 
    /* Find the row containing the n-th term.  */
    unsigned long long cumulative = 0;   /* Σ Fib(i+1) for processed rows */
    unsigned long long len = 0;          /* length of current row = Fib(row+1) */
    unsigned long long f_prev = 0;       /* Fib(row-1) */
    unsigned long long f_curr = 1;       /* Fib(row)   */
    idx_t row;
 
    for (row = 1; ; ++row) {
        len = f_prev + f_curr;          /* Fib(row+1) */
        if (cumulative + len >= n)      /* row found */
            break;
        cumulative += len;
        /* advance Fibonacci values for next row */
        unsigned long long next = len;   /* Fib(row+1) */
        f_prev = f_curr;
        f_curr = next;
    }
 
    /* Position of the term within its row */
    unsigned long long k = (unsigned long long)n - cumulative;
 
    /* If the term is the last in the row, it equals Fib(row+1) */
    if (k == len)
        return (seq_t)len;
 
    /* Compute (-k * Fib(row-1)) mod Fib(row+1) */
    unsigned long long prod = k * f_prev;
    unsigned long long mod = prod % len;
    unsigned long long val = len - mod;
    if (val == len)
        val = 0;   /* handles the case where prod is divisible by len */
 
    return (seq_t)val;
}
