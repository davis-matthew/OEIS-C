/*
 * A192098  Number of distinct (unordered) pairs of partitions of an 8-element
 *          set that have Rand distance n.
 * https://oeis.org/A192098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by exhaustive enumeration of all partitions of an 8‑element set.
   For each unordered pair of partitions we compute the Rand distance
   (the Hamming distance between their 28‑bit membership masks) and
   accumulate the number of pairs that have each possible distance. */
#include <stdint.h>
#include <stdbool.h>

static uint32_t pair_bit[8][8];   /* map pair (i,j) to a bit index 0..27   */
static uint32_t masks[4140];      /* bitmask for each partition          */
static uint64_t distances[29];    /* counts for distances 0..28          */
static bool initialized = false; /* be lazy and precompute once          */

/* Recursive generation of Bell(8)=4140 partitions. */
static void gen_partitions(int pos, int maxblock, int block[8], int *cnt) {
    if (pos == 8) {
        /* build 28‑bit mask of pairs that are in the same block   */
        uint32_t m = 0;
        for (int i = 0; i < 7; ++i)
            for (int j = i + 1; j < 8; ++j)
                if (block[i] == block[j])
                    m |= (uint32_t)1 << pair_bit[i][j];
        masks[(*cnt)++] = m;
        return;
    }
    /* try putting element 'pos' into each existing block 0..maxblock */
    for (int b = 0; b <= maxblock; ++b) {
        block[pos] = b;
        gen_partitions(pos + 1, maxblock, block, cnt);
    }
    /* create a new block for element 'pos' */
    block[pos] = maxblock + 1;
    gen_partitions(pos + 1, maxblock + 1, block, cnt);
}

/* Precomputation performed once. */
static void precompute(void) {
    /* map each pair (i,j) with i<j to a bit index 0..27 */
    int idx = 0;
    for (int i = 0; i < 7; ++i)
        for (int j = i + 1; j < 8; ++j)
            pair_bit[i][j] = idx++;

    /* generate all partitions */
    int part_cnt = 0;
    int block[8] = {0,0,0,0,0,0,0,0};
    gen_partitions(1, 0, block, &part_cnt);

    /* initialise distance counters */
    for (int i = 0; i < 29; ++i)
        distances[i] = 0;

    /* count unordered pairs of partitions by Rand distance */
    for (int i = 0; i < part_cnt; ++i) {
        for (int j = i + 1; j < part_cnt; ++j) {
            uint32_t diff = masks[i] ^ masks[j];
            uint32_t t = diff;
            int d = 0;
            while (t) {
                t &= t - 1; /* remove lowest set bit */
                d++;
            }
            distances[d]++;   /* d is 0..28 */
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) precompute();
    if (n < 1 || n >= 29) return 0;
    return (seq_t)distances[(int)n];
}
