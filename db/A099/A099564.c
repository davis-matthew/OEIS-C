/*
 * A099564  a(0) = 0; for n > 0, a(n) = final nonzero number in the sequence
 *          n, f(n,2), f(f(n,2),3), f(f(f(n,2),3),4),..., where
 *          f(n,d)=Floor(n/F(d+1)), with F denoting the Fibonacci numbers
 *          (A000045).
 * https://oeis.org/A099564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = final non‑zero number in the chain * n, floor(n/F3),
 *            floor(floor(n/F3)/F4), floor(.../F5), … * where Fi is the
 *            Fibonacci sequence with F0=0, F1=1. * a(0) = 0, a(n>0) computed
 *            by successive integer divisions
 * Range:     u8 n <= 437, u16 n <= 3120, u32 n <= 3120, u64 n <= 3120,
 *            u128 n <= 3120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fibonacci table (F0 = 0, F1 = 1, F2 = 1, ...) */
    static seq_t fib[200];
    static int init = 0;
    if (!init) {
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)1;
        for (unsigned i = 3; i < 200; ++i)
            fib[i] = fib[i-1] + fib[i-2];
        init = 1;
    }

    if (n == (idx_t)0) return (seq_t)0;

    seq_t cur = (seq_t)n;
    unsigned idx = 3;          /* start with F3 = 2 */
    while (1) {
        seq_t next = cur / fib[idx];
        if (next == (seq_t)0) break;
        cur = next;
        ++idx;
    }
    return cur;
}
