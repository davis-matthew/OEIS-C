/*
 * A104324  The Fibonacci word over the nonnegative integers; or, the number
 *          of runs of identical bits in the binary Zeckendorf representation
 *          of n.
 * https://oeis.org/A104324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 8812, u16 n <= 8812, u32 n <= 8812, u64 n <= 8812,
 *            u128 n <= 8812, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104324: number of runs of identical bits in the Zeckendorf
   (Fibonacci base) representation of n.  For each n ≥ 0, a(n)
   is the number of consecutive blocks of 0 s and 1 s in the binary
   Zeckendorf expansion, as defined by the OEIS entry. */
#include <stdint.h>
#include <stdbool.h>

static uint64_t fib_tbl[94];   /* 1‑based Fibonacci numbers, F(1)=1, F(2)=2 */
static bool fib_inited = false;

static void init_fib(void)
{
    if (fib_inited) return;
    fib_tbl[1] = 1ULL;
    fib_tbl[2] = 2ULL;
    for (int i = 3; i <= 93; ++i) fib_tbl[i] = fib_tbl[i-1] + fib_tbl[i-2];
    fib_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)0;

    init_fib();

    /* Find the largest Fibonacci number ≤ n */
    int idx = 93;
    while (fib_tbl[idx] > n) --idx;

    seq_t runs = 0;
    bool prev_bit = false;
    bool started = false;
    uint64_t m = n;

    for (int i = idx; i >= 1; --i) {
        bool bit = (m >= fib_tbl[i]);
        if (bit) m -= fib_tbl[i];
        if (!started || bit != prev_bit) {
            ++runs;
            started = true;
        }
        prev_bit = bit;
    }
    return runs;
}
