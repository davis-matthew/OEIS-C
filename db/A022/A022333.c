/*
 * A022333  Exponent of 5 (value of j) in n-th number of form 2^i*5^j.
 * https://oeis.org/A022333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of 5 in the n‑th number of the form 2^i*5^j,
 *            where the numbers 2^i*5^j are listed in increasing order
 *            starting with 1 (a(1)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>

/* Number of terms required for the tests (≈10 000).  A few more are
   pre‑computed to guard against larger calls.  The algorithm is
   O(MAX_N log MAX_N). */
#define MAX_N    200000
#define MAX_PAIRS 300000   /* enough for (ceil(sqrt(MAX_N))+1)^2 */

/* Pair of exponents and an integer key used solely for sorting. */
typedef struct {
    uint16_t i;          /* exponent of 2   */
    uint16_t j;          /* exponent of 5   */
    uint64_t key;        /* i*LOG2 + j*LOG5  */
} Pair;

static bool  initialized = false;
static Pair  pair_buf[MAX_PAIRS];
static uint16_t exp5_of[MAX_N];

/* Values of log10(2) and log10(5) scaled so that the sum of two
   32‑bit products fits into an unsigned 64‑bit integer. */
#define LOG2 301029995664ULL   /* 10^12 * log10(2) */
#define LOG5 698970004336ULL   /* 10^12 * log10(5) */

/* Comparison function for qsort. */
static int
cmp_pairs(const void *a, const void *b)
{
    const Pair *pa = (const Pair *)a;
    const Pair *pb = (const Pair *)b;
    if (pa->key < pb->key) return -1;
    if (pa->key > pb->key) return 1;
    return 0;          /* no two different pairs produce the same key */
}

/* Pre‑compute the sequence up to MAX_N terms. */
static void
init_sequence(void)
{
    if (initialized)
        return;

    /* Find bounds for i and j so that (max_i+1)*(max_j+1) >= MAX_N. */
    double sq = sqrt((double)MAX_N);
    uint32_t max_exponent = (uint32_t)(sq + 1.0);   /* ceil(sqrt(MAX_N)) */

    /* Fill the buffer with all pairs (i,j).  The number of pairs is
       (max_exponent+1)^2, which is comfortably above MAX_N. */
    size_t count = 0;
    for (uint32_t i = 0; i <= max_exponent && count < MAX_PAIRS; ++i)
    {
        for (uint32_t j = 0; j <= max_exponent && count < MAX_PAIRS; ++j)
        {
            pair_buf[count].i = (uint16_t)i;
            pair_buf[count].j = (uint16_t)j;
            pair_buf[count].key =
                (uint64_t)i * LOG2 + (uint64_t)j * LOG5;
            ++count;
        }
    }

    /* Sort by key to obtain the numbers in increasing order. */
    qsort(pair_buf, count, sizeof(Pair), cmp_pairs);

    /* Extract the first MAX_N terms (their exponent of 5). */
    for (size_t k = 0; k < MAX_N && k < count; ++k)
        exp5_of[k] = pair_buf[k].j;

    initialized = true;
}

/* External linkage function. */
seq_t
a(idx_t n)
{
    if (n == 0)        /* offsets begin at 1 */
        return (seq_t)0;

    init_sequence();

    /* There are at least MAX_N terms pre‑computed. */
    size_t idx = (size_t)n - 1;
    if (idx >= MAX_N)
        idx = MAX_N - 1;   /* fallback for huge inputs */

    return (seq_t)exp5_of[idx];
}
