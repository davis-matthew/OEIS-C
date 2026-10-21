/*
 * A217400  Numbers starting with 7.
 * https://oeis.org/A217400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (62*10^floor(log10(9*n-8)) - 8)/9
 * Range:     u8 n <= 11, u16 n <= 1111, u32 n <= 1111, u64 n <= 1111,
 *            u128 n <= 1111, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a digit‑length approach instead of evaluating the
 * fractional power explicitly.  It finds the smallest digit length d
 * satisfying 10^d ≥ 9*n + 1, then returns 7*10^(d-1) plus an offset
 * within that block.  This produces the same sequence of numbers whose
 * decimal representation starts with the digit 7. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle the offset; the sequence is defined for n ≥ 1. */
    if (n == 0) return 0;          /* undefined behaviour for n==0 */

    /* Find the minimal digit length d such that 10^d ≥ 9*n + 1. */
    seq_t target = (seq_t)n * 9 + 1;   /* 9*n + 1  */
    idx_t d = 0;
    seq_t pow10_d = pow10(d);
    while (pow10_d < target) {
        ++d;
        pow10_d = pow10(d);
    }

    /* Compute the cumulative count of numbers with fewer digits (C_{d-1}). */
    seq_t C_prev;
    if (d == 1) {
        C_prev = 0;
    } else {
        seq_t pow10_dm2 = pow10(d - 2);   /* 10^(d-2) */
        C_prev = ( (seq_t)10 * pow10_dm2 - 1 ) / 9;  /* (10*10^(d-2)-1)/9 */
    }

    /* Offset of the desired number within the block of length d. */
    seq_t offset = (seq_t)n - C_prev - 1;

    /* Base number of the block: 7 * 10^(d-1). */
    seq_t base = (seq_t)7 * pow10(d - 1);

    return base + offset;
}
