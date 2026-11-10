/*
 * A072661  Composition of the A059905 and A048679, i.e., a(n) =
 *          A059905(A048679(n)).
 * https://oeis.org/A072661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A059905(A048679(n)); A048679(n) = Zeckendorf (fibbinary)
 *            form of n with each 0 before a non-leading 1 deleted (01 -> 1);
 *            A059905(m) = bits of m at even positions (from LSB) packed
 *            together
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 97 terms through n = 96 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    enum { K = 92 };
    uint64_t fib[K];
    int z[K];
    int c[K];
    int k, L = 0, top = -1, i;
    uint64_t rem = (uint64_t)n;
    seq_t res = 0;

    fib[0] = 1;
    fib[1] = 2;
    for (k = 2; k < K; k++) fib[k] = fib[k - 1] + fib[k - 2];

    for (k = 0; k < K; k++) z[k] = 0;
    for (k = K - 1; k >= 0; k--) {
        if (fib[k] <= rem) {
            z[k] = 1;
            rem -= fib[k];
            if (top < 0) top = k;
        }
    }
    if (top < 0) return (seq_t)0;

    /* compress: drop each 0 immediately before a 1 (non-leading) */
    for (k = top; k >= 0; k--) {
        if (k < top && z[k] == 0 && k > 0 && z[k - 1] == 1) continue;
        c[L++] = z[k];
    }

    /* take bits at even positions from LSB, MSB first (Horner) */
    for (i = 0; i < L; i++) {
        int p = L - 1 - i;
        if ((p & 1) == 0) {
            res = res * 2 + (seq_t)c[i];
        }
    }
    return res;
}
