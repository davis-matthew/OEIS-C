/*
 * A095734  Asymmetricity-index for Zeckendorf-expansion A014417(n) of n.
 * https://oeis.org/A095734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A095734 – Asymmetricity‑index for Zeckendorf‑expansion A014417(n)
 *
 *  The value a(n) is the minimal number of bit flips (changing 0↔1) needed
 *  to convert the Zeckendorf representation of n into a palindrome.
 *  For n=0 the representation is empty and a(0)=0.
 *
 *  Algorithm:
 *      1. Generate all Fibonacci numbers F[0]=1, F[1]=2, … that are ≤ n.
 *      2. Build the Zeckendorf bit array by greedily subtracting the
 *         largest Fibonacci number possible (no two consecutive Fibonacci
 *         numbers are used).
 *      3. Count mismatches between symmetric positions of the bit array;
 *         each mismatch requires one flip.
 *
 *  Complexity:  O(k) where k is the number of Fibonacci numbers ≤ n
 *                (k≤~90 for 64‑bit n).  Well below the limits for n up to
 *                10000.  The algorithm is order‑independent and can be
 *                called with arbitrary n.
 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case: n = 0 has no bits => already a palindrome */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Fibonacci numbers up to n.  90 entries are enough for any 64‑bit n. */
    idx_t fib[90] = {0};
    size_t m = 0;                 /* number of Fibonacci numbers stored */

    /* F[0] = 1 is always in the list when n ≥ 1 */
    fib[m++] = (idx_t)1;

    /* Generate subsequent Fibonacci numbers until the next one exceeds n */
    idx_t prev = fib[m - 1];      /* 1 */
    idx_t cur  = (idx_t)2;        /* F[1] */

    if (cur <= n) {
        fib[m++] = cur;
    }
    /* Build the rest */
    while (cur <= n) {
        idx_t next = prev + cur;
        if (next > n) break;
        prev = cur;
        cur  = next;
        fib[m++] = cur;
    }

    /* Zeckendorf bit pattern of n – length m */
    unsigned char bits[90] = {0};
    idx_t rem = n;
    for (size_t i = m; i > 0; ) {
        --i;
        if (rem >= fib[i]) {
            bits[i] = 1;
            rem -= fib[i];
        }
    }

    /* Count mismatches between mirrored bits */
    seq_t res = (seq_t)0;
    for (size_t i = 0; i < m / 2; ++i) {
        if (bits[i] != bits[m - 1 - i]) {
            ++res;
        }
    }
    return res;
}
