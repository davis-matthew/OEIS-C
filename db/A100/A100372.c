/*
 * A100372  Build up the least positive nonprime number from all subsets of
 *          decimal digits {0,1,2,3,4,5,6,7,8,9}. The terms are ordered as
 *          follows: 1. for fixed k, the k-digit-subsets for design are
 *          ordered lexicographically; 2. choose k=1,2,...,9,10-subsets.
 * https://oeis.org/A100372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least positive nonprime number that uses every
 * digit of a given subset of {0,1,2,3,4,5,6,7,8,9}.  
 * The sequence a(n) enumerates these values by choosing subsets in
 * increasing size k=1..10 and within each size in lexicographic order.
 * For each subset the smallest integer that contains each digit of the
 * subset at least once, uses only digits from the subset, and is
 * nonprime (1 is treated as nonprime) is returned. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.
 * All numbers produced by this sequence are < 10^10, so this
 * test is sufficiently fast. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Depth‑first search to find the lexicographically smallest number
 * of length len that uses only digits in 'digits', contains each
 * digit of the subset at least once, and is nonprime. */
static bool dfs_find(int pos, seq_t num, uint16_t usedMask,
                     int len, const int *digits, int k,
                     uint16_t fullMask, seq_t *out)
{
    if (pos == len) {
        if (usedMask == fullMask) {
            uint64_t val = (uint64_t)num;
            if (!is_prime_uint64(val)) { *out = num; return true; }
        }
        return false;
    }

    /* Quick pruning: if remaining positions cannot cover all missing digits */
    int missing = __builtin_popcount((unsigned int)(fullMask ^ usedMask));
    if (len - pos < missing) return false;

    for (int i = 0; i < k; ++i) {
        int d = digits[i];
        if (pos == 0 && d == 0) continue;        /* no leading zero */
        uint16_t newMask = usedMask | (1u << d);
        seq_t newNum = num * 10 + d;
        if (dfs_find(pos + 1, newNum, newMask, len,
                     digits, k, fullMask, out))
            return true;
    }
    return false;
}

/* For a given digit subset mask (bits 0‑9), find the least nonprime
 * number that uses all digits of the subset.  The set {0} is skipped
 * elsewhere. */
static seq_t find_min_composite(uint32_t mask)
{
    int digits[10];
    int k = 0;
    for (int d = 0; d <= 9; ++d)
        if (mask & (1u << d))
            digits[k++] = d;

    /* Single‑digit subsets */
    if (k == 1) {
        int d = digits[0];
        if (d == 1) return (seq_t)1;           /* 1 is nonprime */
        if (!is_prime_uint64((uint64_t)d))
            return (seq_t)d;                   /* composite digit or 1 */
        return (seq_t)(d * 11);                /* two copies for primes 2,3,5,7 */
    }

    uint16_t fullMask = 0;
    for (int i = 0; i < k; ++i)
        fullMask |= 1u << digits[i];

    seq_t result = 0;
    bool found = false;

    /* Search lengths from k upwards; a solution always occurs
     * within a few extra digits. */
    for (int len = k; len <= k + 10 && !found; ++len) {
        if (dfs_find(0, 0, 0, len, digits, k, fullMask, &result))
            found = true;
    }
    return result;
}

/* Generate all k‑digit subsets in lexicographic order and store
 * their minimal composite in 'seq'.  'idx' tracks the current
 * position in the storage array. */
static void gen_subsets(int start, int need, uint32_t mask,
                        int k, seq_t *seq, int *idx)
{
    if (need == 0) {
        if (mask == (1u << 0))          /* skip the single {0} subset */
            return;
        seq[*idx] = find_min_composite(mask);
        ++(*idx);
        return;
    }
    for (int d = start; d <= 9; ++d)
        gen_subsets(d + 1, need - 1, mask | (1u << d), k, seq, idx);
}

/* Public function requested by OEIS.  It is memoised on first call
 * and returns the n‑th term (1‑based).  Out‑of‑range indices return 0. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        cache = (seq_t *)malloc(1022 * sizeof(seq_t));
        int idx = 0;
        for (int k = 1; k <= 10; ++k)
            gen_subsets(0, k, 0, k, cache, &idx);
        cache_size = idx;   /* should be 1022 */
    }

    if (n == 0 || n > cache_size)
        return 0;           /* undefined for out‑of‑range */
    return cache[n - 1];
}
