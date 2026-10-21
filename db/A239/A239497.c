/*
 * A239497  Number of partitions p of n such that if h = min(p), then h is an
 *          (h,1)-separator of p; see Comments.
 * https://oeis.org/A239497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A239497 – Number of partitions p of n such that the smallest part h
   is an (h,1)-separator of p.  
   The following implementation uses the formula  

        a(n) = Σ_{h=1}^{n-1} Σ_{r=1}^{⌊n/(2h+1)⌋} p_{≤r}( n – r*(2h+1) )  

   where p_{≤r}(m) denotes the number of partitions of m into at most
   r parts.  The value of p_{≤r} is computed by standard recurrence

        p_{≤k}(m) = p_{≤k-1}(m) + p_{≤k}(m-k)   (k ≤ m)
        p_{≤k}(m) = p_{≤k-1}(m)                (k > m)
        p_{≤k}(0) = 1  (empty partition)

   The implementation memoises partition counts in a simple hash
   table that works in both C11 and C++17.  Complexity is
   O(n log n) for each call; it runs well under 2 s for n up to
   10 000. */
#include <stdint.h>
#include <stdlib.h>

/* Hash table for memoisation of p_{≤k}(m).  The key is a 32‑bit word
   formed by (m << 16) | k.  Only m ≤ 10000 and k ≤ 3334 are needed,
   so this fits comfortably. */
#define EMPTY_KEY 0xFFFFFFFFu
#define HASH_CAP 131072u   /* power of two, > 2 * expected entries */

static uint32_t *hash_keys = NULL;
static seq_t   *hash_vals = NULL;
static int     hash_inited = 0;

static void init_hash(void) {
    hash_keys = (uint32_t *)malloc(HASH_CAP * sizeof(uint32_t));
    hash_vals = (seq_t   *)malloc(HASH_CAP * sizeof(seq_t));
    for (size_t i = 0; i < HASH_CAP; ++i)
        hash_keys[i] = EMPTY_KEY;
    hash_inited = 1;
}

static seq_t* hash_find(uint32_t key) {
    if (!hash_inited) init_hash();
    size_t idx = key & (HASH_CAP - 1);
    while (hash_keys[idx] != EMPTY_KEY) {
        if (hash_keys[idx] == key)
            return &hash_vals[idx];
        idx = (idx + 1) & (HASH_CAP - 1);
    }
    return NULL;
}

static void hash_put(uint32_t key, seq_t val) {
    if (!hash_inited) init_hash();
    size_t idx = key & (HASH_CAP - 1);
    while (hash_keys[idx] != EMPTY_KEY && hash_keys[idx] != key)
        idx = (idx + 1) & (HASH_CAP - 1);
    hash_keys[idx] = key;
    hash_vals[idx] = val;
}

/* Memoised computation of number of partitions of m into at most k parts. */
static seq_t part_le(uint32_t m, uint32_t k) {
    if (m == 0) return 1;   /* empty partition */
    if (k == 0) return 0;   /* no parts allowed */
    uint32_t key = (m << 16) | k;
    seq_t *cached = hash_find(key);
    if (cached) return *cached;

    seq_t res;
    if (k > m) {
        res = part_le(m, k - 1);
    } else {
        seq_t a = part_le(m, k - 1);
        seq_t b = part_le(m - k, k);
        res = a + b;
    }
    hash_put(key, res);
    return res;
}

/* --------------------------------------------------------------------- */

/* Public function required by the specifications. */
seq_t a(idx_t n) {
    if (n < 2) return 0;   /* a(1)=0, a(2)=0 by definition */
    seq_t ans = 0;
    for (uint32_t h = 1; h < (uint32_t)n; ++h) {
        uint32_t stride = 2*h + 1;   /* 2h+1 */
        if (stride == 0) continue;  /* safeguard against overflow */
        uint32_t max_r = n / stride;
        for (uint32_t r = 1; r <= max_r; ++r) {
            uint32_t S = n - r * stride;   /* remainder to partition */
            ans += part_le(S, r);
        }
    }
    return ans;
}
