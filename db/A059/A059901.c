/*
 * A059901  Partitions encoded by prime factorization. The partition
 *          [P1+P2+P3+...] with P1>=P2>=P3>=... is encoded as 2^P1 * 3^P2 *
 *          5^P3 *...
 * https://oeis.org/A059901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A059900(A059902(n))
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes partition–encoded numbers using canonical order:
   partitions are ordered first by number of parts (shortest first),
   then lexicographically by parts in decreasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

#define MAX_SUM 50                          /* enough for n <= 10000 */

/* First 50 primes – sufficient for the partition lengths needed */
static const uint64_t primesArray[50] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
    127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL,
    167ULL, 173ULL, 179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL,
    211ULL, 223ULL, 227ULL, 229ULL
};

/* Partition counts and cumulative sums */
static idx_t partition_cnt[MAX_SUM + 1];
static idx_t cum_before[MAX_SUM + 2];
static bool tables_initialized = false;

/* Cache of computed values */
static seq_t *cache = NULL;
static uint8_t *filled = NULL;
static size_t cache_size = 0;

/* Forward declarations */
static void init_partition_tables(void);
static void ensure_cache(idx_t n);
static void gen_len(int rem, int max_part, int parts_left, int pos,
                   int *parts, idx_t *counter, idx_t target,
                   seq_t *result, bool *found);

/* ----- initialize partition tables ----- */
static void init_partition_tables(void)
{
    if (tables_initialized) return;

    /* compute partition numbers p[0..MAX_SUM] */
    partition_cnt[0] = 1;
    for (int i = 1; i <= MAX_SUM; ++i) partition_cnt[i] = 0;
    for (int i = 1; i <= MAX_SUM; ++i)
        for (int j = i; j <= MAX_SUM; ++j)
            partition_cnt[j] += partition_cnt[j - i];

    /* cumulative sums: cum_before[s] = sum_{k=0}^{s-1} p[k] */
    cum_before[0] = 0;
    for (int i = 0; i <= MAX_SUM; ++i)
        cum_before[i + 1] = cum_before[i] + partition_cnt[i];

    tables_initialized = true;
}

/* ----- ensure cache has space for n ----- */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_size) return;

    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    uint8_t *new_filled = (uint8_t *)realloc(filled, need * sizeof(uint8_t));

    if (new_cache == NULL || new_filled == NULL) return; /* allocation failed */

    size_t old = cache_size;
    cache_size = need;
    cache = new_cache;
    filled = new_filled;

    if (old < need) {
        memset(cache + old, 0, (need - old) * sizeof(seq_t));
        memset(filled + old, 0, (need - old) * sizeof(uint8_t));
    }
}

/* ----- recursive generator: partitions of 'rem' into 'parts_left' parts,
       each <= max_part, enumerated in canonical order for a fixed number
       of parts (strictly decreasing parts).  Counter counts how many
       partitions have been seen.  */
static void gen_len(int rem, int max_part, int parts_left, int pos,
                    int *parts, idx_t *counter, idx_t target,
                    seq_t *result, bool *found)
{
    if (*found) return;
    if (parts_left == 0) {
        if (rem == 0) {
            if (*counter == target) {
                seq_t val = 1;
                for (int i = 0; i < pos; ++i)
                    val = seq_mul(val, seq_pow((seq_t)primesArray[i], (idx_t)parts[i]));
                *result = val;
                *found = true;
            }
            ++(*counter);
        }
        return;
    }
    int upper = rem < max_part ? rem : max_part;
    for (int part = upper; part >= 1; --part) {
        parts[pos] = part;
        gen_len(rem - part, part, parts_left - 1, pos + 1,
                parts, counter, target, result, found);
        if (*found) return;
    }
}

/* ----- the OEIS sequence function ----- */
seq_t a(idx_t n)
{
    if (!tables_initialized)
        init_partition_tables();

    ensure_cache(n);
    if (filled[(size_t)n])
        return cache[(size_t)n];

    /* determine the summation 's' and position within that summation */
    int s;
    for (s = 0; s <= MAX_SUM; ++s) {
        if (cum_before[s + 1] > n)
            break;
    }
    if (s == 0) { /* empty partition */
        cache[(size_t)n] = 1;
        filled[(size_t)n] = 1;
        return 1;
    }

    idx_t target = n - cum_before[s];   /* 0‑based index inside sum 's' */
    int parts[50];
    idx_t counter = 0;
    bool found = false;
    seq_t value = 1;

    /* generate partitions by increasing length */
    for (int len = 1; len <= s; ++len) {
        gen_len(s, s, len, 0, parts, &counter, target, &value, &found);
        if (found) break;
    }

    cache[(size_t)n] = value;
    filled[(size_t)n] = 1;
    return value;
}
