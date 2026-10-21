/*
 * A062207  a(n) = 2*n^n-1.
 * https://oeis.org/A062207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n^n - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Compute x^e in arbitrary‑precision type seq_t using exponentiation
 * by squaring. Works for all types supporting arithmetic operators
 * '+', '-', '*', and division by 2 on indices (idx_t).            */
static seq_t _pow_seq_t(seq_t x, idx_t e)
{
    seq_t res = 1;
    seq_t base = x;
    while (e != 0)
    {
        if (e & 1ULL)
            res *= base;
        base *= base;
        e >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* OEIS sequence A062207:  a(n) = 2*n^n - 1   (offset 0)            */
seq_t a(idx_t n)
{
    /* 0^0 is defined as 1 for this sequence (so a(0)=1)              */
    seq_t power = _pow_seq_t((seq_t)n, n);
    power += power;   /* multiply by 2 */
    power -= 1;       /* subtract 1 */
    return power;
}
