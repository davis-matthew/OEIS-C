/*
 * A211299  a(n) = number of n-lettered words in the alphabet {1, 2, 3} with
 *          as many occurrences of the substring (consecutive subword) [1, 2,
 *          1] as of [2, 1, 3].
 * https://oeis.org/A211299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n‑letter words over the alphabet {1,2,3} that
 *            contain as many occurrences of the substring 121 as of 213. This
 *            implementation uses dynamic programming on the suffix (last two
 *            letters) and on the difference (count(121) – count(213))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const uint8_t next_state[9][3] = {
    /* state index: a*3 + b  (a,b are 0‑based for letters 1,2,3) */
    /* a=0,b=0  ->  (1,1) */
    {0, 3, 6},   /* from (1,1): (1,1),(1,2),(1,3) */
    /* a=0,b=1  ->  (1,2) */
    {3, 6, 0},   /* from (1,2): (2,1),(2,2),(2,3) */
    /* a=0,b=2  ->  (1,3) */
    {6, 0, 3},   /* from (1,3): (3,1),(3,2),(3,3) */
    /* a=1,b=0  ->  (2,1) */
    {1, 4, 7},   /* from (2,1): (1,1),(1,2),(1,3) */
    /* a=1,b=1  ->  (2,2) */
    {4, 7, 1},   /* from (2,2): (2,1),(2,2),(2,3) */
    /* a=1,b=2  ->  (2,3) */
    {7, 1, 4},   /* from (2,3): (3,1),(3,2),(3,3) */
    /* a=2,b=0  ->  (3,1) */
    {2, 5, 8},   /* from (3,1): (1,1),(1,2),(1,3) */
    /* a=2,b=1  ->  (3,2) */
    {5, 8, 2},   /* from (3,2): (2,1),(2,2),(2,3) */
    /* a=2,b=2  ->  (3,3) */
    {8, 2, 5}    /* from (3,3): (3,1),(3,2),(3,3) */
};

static const int8_t delta[9][3] = {
    { 0,  0,  0},   /* (1,1)   */
    { 1,  0,  0},   /* (1,2)   121 occurs with next letter 1 */
    { 0,  0,  0},   /* (1,3)   */
    { 0,  0, -1},   /* (2,1)   213 occurs with next letter 3 */
    { 0,  0,  0},   /* (2,2)   */
    { 0,  0,  0},   /* (2,3)   */
    { 0,  0,  0},   /* (3,1)   */
    { 0,  0,  0},   /* (3,2)   */
    { 0,  0,  0}    /* (3,3)   */
};

seq_t a(idx_t n)
{
    /* offset 0…2 are trivial */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    /* At least 3 letters are needed for a non‑trivial DP */
    idx_t k   = n - 2;                       /* number of new letters to add */
    int diff_range = (int)(2 * k + 1);        /* -k … +k mapped to 0 … 2k */
    int offset   = (int)k;                   /* index that represents difference 0 */

    /* Allocate two working layers.  Each layer has size 9 * diff_range */
    size_t layer_size = 9 * (size_t)diff_range;
    seq_t *cur  = (seq_t *)calloc(layer_size, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(layer_size, sizeof(seq_t));
    if (!cur || !next) {
        abort();   /* allocation failed */
    }

    /* Initialise after the first two letters: diff = 0 for every of the 9
       possible states (each pair of letters occurs exactly once). */
    for (int s = 0; s < 9; ++s) {
        cur[s * diff_range + offset] = (seq_t)1;
    }

    /* Perform DP for the remaining k letters. */
    for (int step = 1; step <= k; ++step) {
        /* Zero out the next layer (only the reachable part needs resetting). */
        memset(next, 0, sizeof(seq_t) * layer_size);

        /* Current reachable diff indices are in [offset-step+1, offset+step-1] */
        int cur_min = offset - (step - 1);
        int cur_max = offset + (step - 1);

        for (int s = 0; s < 9; ++s) {
            const seq_t *cs  = cur + s * diff_range;
            seq_t *ns = next;
            for (int d = cur_min; d <= cur_max; ++d) {
                seq_t val = cs[d];
                if (!val) continue;

                int nd0 = d + (int)delta[s][0];
                int nd1 = d + (int)delta[s][1];
                int nd2 = d + (int)delta[s][2];

                ns[next_state[s][0] * diff_range + nd0] += val;
                ns[next_state[s][1] * diff_range + nd1] += val;
                ns[next_state[s][2] * diff_range + nd2] += val;
            }
        }

        /* Swap layers for the next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum over all states for difference 0 (index = offset). */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 9; ++s) {
        result += cur[s * diff_range + offset];
    }

    free(cur);
    free(next);
    return result;
}
