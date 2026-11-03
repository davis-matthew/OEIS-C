/*
 * A055020  Number of iterations of sigma() required until 2n (or greater) is
 *          reached.
 * https://oeis.org/A055020
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k such that σ^k(n) ≥ 2n, where σ(n) is the
 *            sum of divisors of n
 * Range:     u8 n <= 66, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 119 terms through n = 9858 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute σ(n) for  n as idx_t, returning a seq_t value. */
static seq_t sigma_val(idx_t n)
{
    if (n == 0) return 0;  /* not used for the sequence, but safe */

    seq_t result = 1;
    idx_t temp = n;

    /* Handle factor 2 separately for speed */
    if ((temp & 1) == 0) {
        seq_t term  = 1;
        seq_t power = 1;
        while ((temp & 1) == 0) {
            temp >>= 1;
            power *= (seq_t)2;
            term  += power;
        }
        result *= term;
    }

    /* Odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t term  = 1;
            seq_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
                term  += power;
            }
            result *= term;
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        result *= (seq_t)(1 + temp);
    }

    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* behaviour below offset is irrelevant */

    seq_t target = (seq_t)n * 2;   /* 2n */
    seq_t current = (seq_t)n;
    seq_t count = (seq_t)0;

    while (current < target) {
        current = sigma_val((idx_t)current);
        count++;
    }
    return count;
}
