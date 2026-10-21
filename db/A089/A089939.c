/*
 * A089939  T(i,j) = 1 if F(i) AND F(j) = 0, otherwise 0, where F is A003714
 *          and AND is the bitwise logical-and operation. Table read by
 *          antidiagonals.
 * https://oeis.org/A089939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = 1 if the Zeckendorf representations of the pair (i,j),
 *            determined from the anti‑diagonal enumeration, are disjoint;
 *            otherwise 0. F(i) is the binary number whose 1‑bits occur at the
 *            Fibonacci numbers that compose the Zeckendorf representation of
 *            i. Then T(i,j)=1 ⇔ F(i)&F(j)=0 ⇔ the two sets...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* maximum number of Fibonacci numbers needed for 64‑bit indices */
#define MAX_FIB 93

static uint64_t fibs[MAX_FIB];
static size_t fib_cnt;

/* Initialise the Fibonacci table (F[0]=1, F[1]=2,...). */
static void init_fibs(void)
{
    if (fib_cnt != 0) return;          /* already initialised */

    fibs[0] = 1ULL;
    fibs[1] = 2ULL;
    fib_cnt = 2;

    while (fib_cnt + 1 < MAX_FIB) {
        uint64_t a = fibs[fib_cnt - 2];
        uint64_t b = fibs[fib_cnt - 1];
        /* check for overflow: a + b must fit in uint64_t */
        if (UINT64_MAX - a < b) break;
        fibs[fib_cnt++] = a + b;
    }
}

/* Build a two‑word mask representing the Zeckendorf set of n.
   low  holds bits 0..63, high holds bits 64..127. */
static void zeckendorf_mask(idx_t n, uint64_t *low, uint64_t *high)
{
    *low  = 0;
    *high = 0;
    for (int idx = (int)fib_cnt - 1; idx >= 0; --idx) {
        if (n >= fibs[idx]) {
            n -= fibs[idx];
            if (idx < 64)
                *low |= 1ULL << idx;
            else
                *high |= 1ULL << (idx - 64);
        }
    }
}

/* Return the nth term of A089939. */
seq_t a(idx_t n)
{
    init_fibs();   /* ensure Fibonacci table is ready */

    /* Find diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t d = (idx_t)((((double)8 * n + 1.0) * 0.5 * 0.5) - 0.5); /* rough estimate */
    /* Adjust to satisfy the inequality */
    while ( (d * (d + 1)) / 2 > n ) --d;
    while ( ((d + 1) * (d + 2)) / 2 <= n ) ++d;

    idx_t start = (d * (d + 1)) / 2;
    idx_t i = n - start;          /* row index */
    idx_t j = d - i;              /* column index */

    uint64_t low_i, high_i;
    uint64_t low_j, high_j;
    zeckendorf_mask(i, &low_i, &high_i);
    zeckendorf_mask(j, &low_j, &high_j);

    uint64_t inter_low  = low_i  & low_j;
    uint64_t inter_high = high_i & high_j;

    return (inter_low | inter_high) == 0 ? 1 : 0;
}
