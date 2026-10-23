/*
 * A022340  Even Fibbinary numbers (A003714); also a(n) = 2*Fibbinary(n).
 * https://oeis.org/A022340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1596, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2 * (the n‑th non‑negative integer whose binary expansion contains
 * no two consecutive 1's).  The sequence begins: a(0)=0, a(1)=2, a(2)=4, a(3)=8,
 * a(4)=10, a(5)=16, a(6)=18, …
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute enough Fibonacci numbers F(0)=0, F(1)=1. */
    static seq_t fib[200];
    static bool fib_init = false;
    if (!fib_init) {
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i < 200; ++i)
            fib[i] = fib[i-1] + fib[i-2];
        fib_init = true;
    }

    if (n == 0)
        return 0;            /* the only 0 in the sequence */

    /* k is the index among the positive fibbinary numbers (exclude the zero). */
    seq_t k = (seq_t)n - 1;   /* k ≥ 0 */

    /* Find the minimal length L (≥1) for which the number of admissible
       binary strings of length ≤ L with first bit 1 exceeds k.
       Sum_{i=1..L} F(i) = F(L+2) – 1. */
    size_t L = 1;
    while (fib[L+2] - 1 <= k)
        ++L;

    /* Rank r inside the block of numbers with exactly L bits. */
    seq_t r = k - (fib[L+1] - 1);   /* 0‑based rank within this block */

    /* Construct the L‑bit fibbinary number (first bit is always 1). */
    seq_t x = (seq_t)1 << (L-1);     /* leading 1 */
    int prev = 1;                   /* previous bit to the left of the current position */

    for (int pos = (int)L-2; pos >= 0; --pos) {
        seq_t count0 = fib[pos+2];  /* number of completions if we put a 0 here */

        if (prev == 1) {            /* must place 0 */
            prev = 0;
            continue;
        }

        /* prev == 0 */
        if (r < count0) {           /* put 0 */
            prev = 0;
        } else {                    /* put 1, skip the block of zeros */
            x |= ((seq_t)1 << pos);
            r -= count0;
            prev = 1;
        }
    }

    /* The required term is twice the fibbinary number. */
    return x << 1;
}
