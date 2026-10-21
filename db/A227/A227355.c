/*
 * A227355  Product of run lengths in Zeckendorf representation of n.
 * https://oeis.org/A227355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the lengths of consecutive 0‑runs * in the
 *            Zeckendorf representation of n (a(0)=1). The Zeckendorf *
 *            representation is the unique sum of non‑consecutive Fibonacci
 *            numbers * F1=1, F2=2, F3=3, … . The algorithm is O(log n) and
 *            uses only * constant extra space
 * Range:     u8 n <= 9529, u16 n <= 9529, u32 n <= 9529, u64 n <= 9529,
 *            u128 n <= 9529, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Fibonnaci numbers used as the digits of the Zeckendorf binary.
 * They are generated once on the first call. */
static idx_t     fibs[100];
static size_t    fib_cnt  = 0;
static bool      fibs_ready = false;

/* Initialise the Fibonacci array with all fibs that fit in idx_t. */
static void init_fibs(void)
{
    if (fibs_ready)
        return;
    fibs[0] = (idx_t)1;
    fibs[1] = (idx_t)2;
    fib_cnt = 2;
    while (fib_cnt < sizeof(fibs)/sizeof(fibs[0])) {
        idx_t a = fibs[fib_cnt-1];
        idx_t b = fibs[fib_cnt-2];
        /* Check for overflow */
        if (a > (UINT64_MAX - b))
            break;
        fibs[fib_cnt++] = a + b;
    }
    fibs_ready = true;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* a(0) = 1 by definition */
        return (seq_t)1;

    init_fibs();

    idx_t remaining = n;
    seq_t product = (seq_t)1;
    unsigned int zero_run = 0;
    bool started = false;      /* first 1 seen? */

    /* Iterate over Fibonacci indices from largest to 0. */
    for (size_t i = fib_cnt; i-- > 0; ) {
        if (remaining >= fibs[i]) {            /* place a 1 */
            if (started) {
                if (zero_run > 0) {
                    product *= (seq_t)zero_run;
                    zero_run = 0;
                }
            } else {
                started = true;                 /* first 1 encountered */
            }
            remaining -= fibs[i];
        } else {                                 /* place a 0 */
            if (started)
                ++zero_run;
            /* else leading zeros are ignored */
        }
    }

    /* Trailing zero run after the last 1, if any. */
    if (started && zero_run > 0)
        product *= (seq_t)zero_run;

    return product;
}
