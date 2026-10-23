/*
 * A020899  Numbers k with an odd number of terms in their Zeckendorf
 *          representation (write k as a sum of non-consecutive distinct
 *          Fibonacci numbers).
 * https://oeis.org/A020899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n‑th natural number whose Zeckendorf representation
   has an odd number of summands. */
#include <stdbool.h>
#include <stddef.h>

/* List of Fibonacci numbers starting with F1=1, F2=2.
   F93 > 2^64 so this array is large enough for any 64‑bit idx_t. */
#define MAX_FIB 93
static idx_t fibs[MAX_FIB];
static size_t fib_len = 0;

/* Ensure that fibs[] contains all Fibonacci numbers up to at least 'limit'.
   The array is filled on first call and extended only when necessary. */
static void ensure_fibs(idx_t limit)
{
    if (fib_len == 0) {
        fibs[0] = 1;
        fibs[1] = 2;
        fib_len = 2;
    }
    while (fib_len < MAX_FIB && fibs[fib_len - 1] <= limit) {
        fibs[fib_len] = fibs[fib_len - 1] + fibs[fib_len - 2];
        fib_len++;
    }
}

/* Return true iff the Zeckendorf representation of 'n' uses an odd
   number of Fibonacci summands. */
static bool odd_zeng(idx_t n)
{
    if (n == 0)
        return false;           /* 0 has no summands – even parity  */
    ensure_fibs(n);
    bool parity = false;        /* false = even, true = odd      */
    while (n > 0) {
        /* find the largest Fibonacci number <= n */
        size_t i = fib_len;
        while (i > 0 && fibs[i - 1] > n)
            --i;
        n -= fibs[i - 1];
        parity = !parity;       /* flip parity for each summand   */
    }
    return parity;
}

/* External interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                   /* offset is 1; return 0 safely  */
        return (seq_t)0;
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        if (odd_zeng(k)) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
