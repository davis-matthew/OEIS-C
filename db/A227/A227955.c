/*
 * A227955  Triangle read by rows, T(n, k) =
 *          prime(1)^p(k,1)*...*prime(n)^p(k,n) where p(k,j) is the j-th part
 *          of the k-th partition of n. The partitions of n are ordered in
 *          reversed lexicographic order read from left-to-right, starting
 *          with [1,1,...1] going down to [n].
 * https://oeis.org/A227955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = product_{i=1}^{L} prime(i)^{p_i}, where p_i is the i‑th
 *            part of the (pos)-th partition of the row containing index n,
 *            written in reversed lexicographic order
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 96, u64 n <= 683,
 *            u128 n <= 9295, big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

#define MAX_ROW 500     /* sufficient for any index ≤ 10000          */
#define PRIME_LIMIT 2000/* ensure enough primes for first MAX_ROW  */

static bool initialized = false;
static uint64_t dp_leq[MAX_ROW+1][MAX_ROW+1];
static uint64_t part_cnt[MAX_ROW+1];
static uint64_t *prime_array = NULL;
static size_t prime_cnt = 0;

/* Initialize global tables                                            */
static void init(void)
{
    /* 1. Count partitions and DP for partitions with parts ≤ k   */
    for (int n = 0; n <= MAX_ROW; ++n)
        for (int k = 0; k <= MAX_ROW; ++k)
            dp_leq[n][k] = 0;
    for (int k = 0; k <= MAX_ROW; ++k)
        dp_leq[0][k] = 1;                 /* only the empty partition */

    for (int n = 1; n <= MAX_ROW; ++n)
        dp_leq[n][0] = 0;

    for (int n = 1; n <= MAX_ROW; ++n)
        for (int k = 1; k <= MAX_ROW; ++k)
        {
            uint64_t v = dp_leq[n][k-1];
            if (k <= n)
                v += dp_leq[n-k][k];
            dp_leq[n][k] = v;
        }

    /* 2. Count total partitions for each n                        */
    for (int i = 0; i <= MAX_ROW; ++i)
        part_cnt[i] = 0;
    part_cnt[0] = 1;                     /* p(0) = 1 */
    for (int v = 1; v <= MAX_ROW; ++v)
        for (int j = v; j <= MAX_ROW; ++j)
            part_cnt[j] += part_cnt[j-v];

    /* 3. Build first several primes                                 */
    build_primes_upto((idx_t)PRIME_LIMIT, &prime_array, &prime_cnt);

    initialized = true;
}

/* Main OEIS function                                                   */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    /* Find the row that contains index n                               */
    uint64_t cumulative = 0;
    int row = 0;
    while (cumulative <= (uint64_t)n)
    {
        cumulative += part_cnt[row];
        ++row;
    }
    --row;                                     /* actual row          */
    uint64_t previous_sum = cumulative - part_cnt[row];
    idx_t pos = (idx_t)(n - previous_sum);     /* 0‑based position   */

    /* Special case: row 0 has only the empty partition                */
    if (row == 0)
        return (seq_t)1;

    /* Unrank the pos‑th partition in reversed lexicographic order      */
    int parts[MAX_ROW];
    int len = 0;
    int rem = row;
    int max_allowed = row;

    while (rem > 0)
    {
        int limit = rem < max_allowed ? rem : max_allowed;
        for (int part = 1; part <= limit; ++part)
        {
            uint64_t cnt = dp_leq[rem - part][part];
            if ((uint64_t)pos < cnt)
            {
                parts[len++] = part;
                rem -= part;
                max_allowed = part;
                break;
            }
            pos -= cnt;
        }
    }

    /* Compute the prime product                                       */
    seq_t result = 1;
    for (int i = 0; i < len; ++i)
    {
        if (parts[i] == 0)
            continue;
        seq_t pow_val = big_pow((seq_t)prime_array[i], (seq_t)parts[i]);
        result = result * pow_val;
    }

    return result;
}
