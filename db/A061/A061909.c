/*
 * A061909  Skinny numbers: numbers n such that there are no carries when n is
 *          squared by "long multiplication".
 * https://oeis.org/A061909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice, look
 * Formula:   a(n) is the n-th integer whose square in long * multiplication
 *            produces no carries, i.e. for the decimal * digits d[i] of n the
 *            convolution sums * e_k = Σ_{i=0..k} d[i] * d[k-i] are all ≤ 9
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8777, u64 n <= 8777,
 *            u128 n <= 8777, big any n
 * Helpers:   cmp, get_pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp.h"
#include "../utils/get_pow10.h"

#define MAX_LEN 10
#define MAX_VAL ((seq_t)10000000000ULL)     /* 1e10 is safely above the 10k‑th term */

static uint8_t  digits[MAX_LEN];
static seq_t   results[300000];
static size_t  res_cnt = 0;

/*  Check that all convolution sums e_k  (k = 0 .. 2*len-2) are ≤ 9.  */
static bool valid_len(size_t len)
{
    for (size_t k = 0; k < 2*len - 1; ++k) {
        int sum = 0;
        for (size_t i = 0; i < len; ++i) {
            size_t j = k - i;
            if (j >= len) continue;
            sum += digits[i] * digits[j];
        }
        if (sum > 9) return false;
    }
    return true;
}

/*  Compute numeric value of current digit array (len digits).  */
static seq_t value_len(size_t len)
{
    seq_t val = 0;
    for (size_t i = 0; i < len; ++i)
        val += (seq_t)digits[i] * get_pow10((unsigned int)i);
    return val;
}

/*  DFS generating all numbers up to MAX_LEN digits that satisfy the
 *  “no‑carry” condition.  */
static void dfs(size_t len)
{
    if (len > 0) {
        seq_t val = value_len(len);
        if (val <= MAX_VAL) results[res_cnt++] = val;
        if (val > MAX_VAL) return;            /* further digits only larger */
    }
    if (len == MAX_LEN) return;
    for (int d = 0; d <= 9; ++d) {
        digits[len] = (uint8_t)d;
        if (!valid_len(len+1)) continue;
        dfs(len+1);
    }
}

/*  Build the sequence once.  */
static bool built = false;

seq_t a(idx_t n)
{
    if (!built) {
        dfs(0);                           /* generate all candidates */
        /* sort and remove duplicates */
        qsort(results, res_cnt, sizeof(seq_t), cmp);
        size_t uniq = 0;
        for (size_t i = 0; i < res_cnt; ++i) {
            if (i && results[i] == results[uniq-1]) continue;
            results[uniq++] = results[i];
        }
        res_cnt = uniq;
        built = true;
    }
    if (n == 0 || n > (idx_t)res_cnt) return (seq_t)0;
    return results[(size_t)n - 1];
}
