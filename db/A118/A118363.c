/*
 * A118363  Factorial base Niven (or Harshad) numbers: numbers that are
 *          divisible by the sum of their factorial base digits.
 * https://oeis.org/A118363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth factorial base Niven (Harshad) number. The function
 *            finds the nth positive integer whose factorial‑base expansion is
 *            divisible by the sum of its factorial‑base digits by brute‑force
 *            search
 * Range:     u8 n <= 54, u16 n <= 5565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9803 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t
sum_factorial_digits(seq_t x)
{
    /* static table of factorials fact[i] = i!   (fact[0] = 0! = 1) */
    static seq_t fact[64];
    static int fact_size = 0;  /* number of valid entries in fact[] */

    /* Initialise on first use */
    if (fact_size == 0) {
        fact[0] = (seq_t)1;          /* 0! */
        fact[1] = (seq_t)1;          /* 1! */
        fact_size = 2;               /* fact[0] .. fact[1] stored */
    }

    /* Extend the table until fact[fact_size-1] > x or overflow */
    while (fact[fact_size - 1] <= x) {
        if (fact_size >= 63)         /* avoid array overflow */
            break;
        seq_t next = fact[fact_size - 1] * (seq_t)(fact_size);
        if (next < fact[fact_size - 1])   /* arithmetic overflow */
            break;
        fact[fact_size] = next;
        fact_size++;
    }

    /* Find the largest index i such that fact[i] <= x */
    int i = fact_size - 1;
    while (i > 0 && fact[i] > x)
        i--;

    seq_t remainder = x;
    seq_t sum = 0;
    for (int idx = i; idx >= 1; --idx) {
        seq_t digit = remainder / fact[idx];
        remainder -= digit * fact[idx];
        sum += digit;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Sequentially search candidate integers */
    idx_t   found = 0;
    seq_t   cand  = (seq_t)1;

    while (found < n) {
        seq_t s = sum_factorial_digits(cand);
        if (s != 0 && (cand % s) == 0) {
            found++;
            if (found == n)
                return cand;
        }
        cand++;
    }

    /* Unreachable, but keeps the compiler happy */
    return (seq_t)0;
}
