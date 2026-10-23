/*
 * A165195  Rows of triangle A165194 tend to this sequence; generated from
 *          A000110.
 * https://oeis.org/A165195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(popcount( (n-1) xor ((n-1)>>1) ))
 * Range:     u8 n <= 85, u16 n <= 682, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = A000110( A005811(n-1) )                           */
/* where A005811(k) = popcount( Gray(k) ) (Gray code of k)   */

#include <stdint.h>
#include <stdbool.h>

/* ----------  helper: popcount --------------------------------- */
static int popcount_uint64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

/* ----------  helper: binomial (n choose k, 0 <= k <= n <= 30) --- */
static unsigned long long binomial_small(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned long long res = 1;
    for (int i = 1; i <= k; ++i)
    {
        res = (res * (unsigned long long)(n - k + i)) / (unsigned long long)i;
    }
    return res;
}

/* ----------  helper: pre‑compute Bell numbers up to 31 ------------ */
static seq_t bell[32];
static bool bell_inited = false;

static void init_bell(void)
{
    bell[0] = 1;
    for (int n = 1; n < 32; ++n) {
        seq_t sum = 0;
        for (int k = 0; k < n; ++k) {
            unsigned long long bin = binomial_small(n - 1, k);
            sum += (seq_t)bin * bell[k];
        }
        bell[n] = sum;
    }
}

/* ----------  main function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined offset, keep safe */
    if (!bell_inited) {            /* compute Bell numbers once */
        init_bell();
        bell_inited = true;
    }
    uint64_t k   = (uint64_t)n - 1;          /* n-1, fits into 64 bits */
    uint64_t gray = k ^ (k >> 1);            /* Gray code of k      */
    int   idx   = popcount_uint64(gray);     /* index for Bell number */

    return bell[idx];
}
