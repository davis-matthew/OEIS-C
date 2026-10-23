/*
 * A123564  The infinite Fibonacci word reencoded by writing successive
 *          non-overlapping pairs of bits as decimal numbers.
 * https://oeis.org/A123564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*A005614(2n-1) + A005614(2n) where A005614 is the
 *            infinite Fibonacci word (0/1) with w0 = "1", w1 = "10", w_{k+1}
 *            = w_k w_{k-1}
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary includes */
#include <stdint.h>

/* --------------------  Fibonacci word utilities  -------------------- */

static uint64_t g_fib[94];          /* Fibonacci lengths: g_fib[1]=1, g_fib[2]=2 */
static unsigned g_fib_cnt = 0;      /* number of Fibonacci numbers stored */
static int g_fib_init = 0;

/* Initialize the Fibonacci length table once. */
static void init_fib(void)
{
    if (g_fib_init) return;
    g_fib[1] = 1ULL;
    g_fib[2] = 2ULL;
    g_fib_cnt = 2U;
    while (1) {
        uint64_t a = g_fib[g_fib_cnt - 1];
        uint64_t b = g_fib[g_fib_cnt];
        if (UINT64_MAX - a < b) break;          /* would overflow */
        g_fib[g_fib_cnt + 1] = a + b;
        g_fib_cnt++;
        if (g_fib[g_fib_cnt] > UINT64_MAX) break;
    }
    g_fib_init = 1;
}

/* Return the k-th bit (1-indexed) of the infinite Fibonacci word.
   Bits are 0 or 1.  Recursive descent uses the fact that
   w_m = w_{m-1} w_{m-2} and that |w_m| = g_fib[m]. */
static int fib_bit(idx_t k)
{
    if (k == 1ULL) return 1;
    if (k == 2ULL) return 0;

    /* find the smallest m with g_fib[m] >= k */
    unsigned m = 1U;
    while (g_fib[m] < k) m++;

    if (k <= g_fib[m - 1]) {
        /* first part: same index in the shorter word */
        return fib_bit(k);
    } else {
        /* second part: index in the preceding word */
        return fib_bit(k - g_fib[m - 1]);
    }
}

/* --------------------  Sequence a(n)  -------------------- */

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1; for n==0 return 0 */
    if (n == 0ULL) return (seq_t)0;

    /* Initialise the Fibonacci tables lazily. */
    init_fib();

    /* positions of the two bits used in a(n) */
    idx_t pos1 = 2 * n - 1;
    idx_t pos2 = 2 * n;

    int b1 = fib_bit(pos1);
    int b2 = fib_bit(pos2);

    return (seq_t)(2 * b1 + b2);
}
