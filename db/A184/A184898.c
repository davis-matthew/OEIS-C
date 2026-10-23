/*
 * A184898  a(n) = C(2n,n) * (8^n/n!^2) * Product_{k=0..n-1} (8k+1)*(8k+7).
 * https://oeis.org/A184898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A184898
   a(n) = C(2n,n) * (8^n/n!^2) * ∏_{k=0}^{n-1} (8k+1)(8k+7)
   a(0) = 1
   Recurrence: a(0)=1
   a(n) = a(n-1) * 8 * (2n)(2n-1)(8n-7)(8n-1) / n^4   (for n≥1)
   All divisions are exact for the sequence. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t val = (seq_t)1;

    /* Use 128‑bit intermediate for built‑in integer types of 64 bits or less.
       For larger (arbitrary‑precision) types just use the type's own arithmetic. */
    if (sizeof(seq_t) <= 8) {
        for (idx_t i = 1; i <= n; ++i) {
            __uint128_t prod = (__uint128_t)val;
            prod *= 8;
            prod *= ( (__uint128_t)2 * i );
            prod *= ( (__uint128_t)2 * i - 1 );
            prod *= ( (__uint128_t)8 * i - 7 );
            prod *= ( (__uint128_t)8 * i - 1 );
            prod /= ( ( (__uint128_t)i * i * i * i ) );
            val = (seq_t)prod;
        }
    } else {
        for (idx_t i = 1; i <= n; ++i) {
            val = val * (seq_t)8;
            val = val * (seq_t)(2 * i);
            val = val * (seq_t)(2 * i - 1);
            val = val * (seq_t)(8 * i - 7);
            val = val * (seq_t)(8 * i - 1);
            /* i^4 fits in 64‑bit for n ≤ 10000 */
            uint64_t denom = (uint64_t)i * i * i * i;
            val = val / (seq_t)denom;
        }
    }

    return val;
}
