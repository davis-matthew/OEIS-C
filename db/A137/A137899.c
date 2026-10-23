/*
 * A137899  Number of partitions of n into products of at least two
 *          consecutive positive integers.
 * https://oeis.org/A137899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into sums of products * of at
 *            least two consecutive positive integers. The function * computes
 *            all such products up to n, removes duplicates, and then * uses
 *            an integer‐partition dynamic program to count the number * of
 *            unordered sums that achieve n. The algorithm...
 * Range:     u8 n <= 77, u16 n <= 241, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* comparison function for qsort on idx_t values */
static int cmp_idx_t(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va > vb) - (va < vb);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n < 2) {
        return (seq_t)0;          /* only n=0 or n=1 yield zero */
    }

    /* ---------- generate all distinct products of consecutive integers ----------
     * For each starting integer m >= 1, multiply successive integers
     * until the product exceeds n.  Keep only products of length >= 2. */
    size_t cap = 64;
    idx_t *products = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!products) return (seq_t)0;
    size_t cnt = 0;

    for (idx_t m = 1; m <= n; ++m) {
        unsigned __int128 prod = (unsigned __int128)m * (m + 1);   /* length 2 */
        if (prod > n) break;                  /* future m will only be larger */
        idx_t len = 2;
        while (prod <= n) {
            /* store product if there is room */
            if (cnt == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(products, cap * sizeof(idx_t));
                if (!tmp) { free(products); return (seq_t)0; }
                products = tmp;
            }
            products[cnt++] = (idx_t)prod;

            /* prepare next multiplication */
            ++len;
            if (prod > n / (unsigned __int128)(m + len - 1)) break;
            prod *= (unsigned __int128)(m + len - 1);
        }
    }

    /* Sort and deduplicate the product list */
    if (cnt == 0) { free(products); return (seq_t)0; }
    qsort(products, cnt, sizeof(idx_t), cmp_idx_t);

    size_t uniq = 1;
    for (size_t i = 1; i < cnt; ++i) {
        if (products[i] != products[uniq - 1]) {
            products[uniq++] = products[i];
        }
    }
    cnt = uniq;                     /* now cnt is the number of unique coins */

    /* ---------- dynamic programming to count partitions ----------
     * dp[i] holds the number of ways to write i as a sum of the collected products.
     * Iterate over each distinct product (coin) and update dp accordingly. */
    size_t dp_size = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) { free(products); return (seq_t)0; }
    dp[0] = (seq_t)1;   /* one way to sum to zero (empty sum) */

    for (size_t ci = 0; ci < cnt; ++ci) {
        idx_t coin = products[ci];
        for (idx_t v = coin; v <= n; ++v) {
            dp[v] = dp[v] + dp[v - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(products);
    return result;
}
