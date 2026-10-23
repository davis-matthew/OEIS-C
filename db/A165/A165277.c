/*
 * A165277  Number of odd-indexed Fibonacci numbers in the Zeckendorf
 *          representation of n.
 * https://oeis.org/A165277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 428, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Zeckendorf representation of n: count of odd‑indexed Fibonacci summands
 *  (odd index in the standard Fibonacci numbering, i.e. F_1 = 1, F_2 = 1, …).
 *  Compute greedily the Zeckendorf sum using F(2)=1, F(3)=2, …, and
 *  increment a counter when the chosen Fibonacci number has an odd index.
 */

/* Include standard types for idx_t (uint64_t) */
#include <stdint.h>

#define MAX_FIBS 100   /* ample for n up to at least 10^19 (F_93 < 2^64) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but harmless */

    /* Pre‑compute Fibonacci numbers F(2)=1, F(3)=2, … up to > n. */
    seq_t fib[MAX_FIBS];
    idx_t len = 0;
    seq_t f_prev = (seq_t)1;        /* F(2) */
    seq_t f_curr = (seq_t)2;        /* F(3) */

    fib[len++] = f_prev;
    if (len < MAX_FIBS) fib[len++] = f_curr;

    while (f_curr <= (seq_t)n && len + 1 < MAX_FIBS) {
        seq_t f_next = f_curr + f_prev;  /* Fibonacci recurrence */
        fib[len++] = f_next;
        f_prev = f_curr;
        f_curr = f_next;
    }

    seq_t rem = (seq_t)n;
    seq_t count = 0;

    for (long long i = (long long)len - 1; i >= 0; --i) {
        if (fib[(idx_t)i] <= rem) {
            rem -= fib[(idx_t)i];
            /* fib[i] corresponds to Fibonacci index i+2.
               Odd index ⇔ (i+2) is odd ⇔ i is odd. */
            if ((idx_t)i % 2 == 1) {
                count += (seq_t)1;
            }
        }
    }

    return count;
}
