/*
 * A031212  Periods of sum of 9th powers of digits iterated until the sequence
 *          becomes periodic.
 * https://oeis.org/A031212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of iterating the sum of the ninth powers of digits starting
   from n.  a(n) is the length of the eventual cycle of the sequence
   starting with x = n and repeatedly replacing x by sum_{d∈digits(x)} d^9. */
/* Disclaimer: this is an O(k) algorithm where k is the total number of
   iterations before the cycle is detected; it works for any seq_t
   that supports the required arithmetic operators. */
#include <stdint.h>
#include <stdbool.h>

/* Pre‑computed 0^9 … 9^9 in the same type as seq_t. */
static const seq_t pow9[10] = {
    0ULL,            /* 0^9 */
    1ULL,            /* 1^9 */
    512ULL,          /* 2^9 */
    19683ULL,        /* 3^9 */
    262144ULL,       /* 4^9 */
    1953125ULL,      /* 5^9 */
    10077696ULL,     /* 6^9 */
    40353607ULL,     /* 7^9 */
    134217728ULL,    /* 8^9 */
    387420489ULL     /* 9^9 */
};

/* Sum of ninth powers of the decimal digits of x. */
static seq_t sum9(seq_t x)
{
    seq_t s = 0;
    if (x == 0) return 0;
    while (x != 0) {
        /* digit is guaranteed to be in [0,9] */
        seq_t d = x % 10ULL;
        /* cast d to an unsigned index; seq_t can convert to unsigned */
        s += pow9[(unsigned int)d];
        x /= 10ULL;
    }
    return s;
}

/* Main function: returns the period length for the sequence starting
   from integer n. */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    /* Euler/Tortoise & Hare algorithm for cycle detection. */
    seq_t tortoise = sum9(start);
    seq_t hare = sum9(tortoise);
    while (tortoise != hare) {
        tortoise = sum9(tortoise);
        hare = sum9(sum9(hare));
    }

    /* Find first element of cycle (mu). */
    tortoise = start;
    seq_t mu = 0;
    while (tortoise != hare) {
        tortoise = sum9(tortoise);
        hare = sum9(hare);
        mu++;
    }

    /* Find cycle length (lambda). */
    seq_t lam = 1;
    hare = sum9(tortoise);
    while (hare != tortoise) {
        hare = sum9(hare);
        lam++;
    }

    return lam;
}
