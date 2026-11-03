/*
 * A048365  a(n)^3 is smallest cube containing exactly n 0's.
 * https://oeis.org/A048365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1, a(1)=0. For n>=2 let k=n/3. If n%3==0: a(n)=10^k. If
 *            n%3==1: a(n)=16*10^k. If n%3==2: a(n)=52*10^k
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048365 – a(n)^3 is smallest cube containing exactly n 0's. */
#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    idx_t k   = n / 3;
    int  rem  = n % 3;
    seq_t base = pow10(k);
    if (rem == 0) return base;
    if (rem == 1) return 16 * base;
    return 52 * base;
}
