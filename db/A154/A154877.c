/*
 * A154877  Numbers that eventually reach the cycle 160-217-352 under "x ->
 *          sum of cubes of digits of x" (see A055012).
 * https://oeis.org/A154877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate numbers that eventually reach the 160-217-352 cycle
   under the transformation x -> sum of cubes of the digits of x. */

#include <stdlib.h>

static int visited_mark[20001] = {0};
static int vis_id = 1;                    /* unique marker for each walk */

static inline seq_t sum_of_cubes(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        seq_t d = x % (seq_t)10;
        sum += d * d * d;
        x   /= (seq_t)10;
    }
    return sum;
}

/* Returns 1 if x eventually reaches 160, 217 or 352, 0 otherwise. */
static int reaches_cycle(seq_t x)
{
    int id = vis_id++;
    while (1) {
        if (x == (seq_t)160 || x == (seq_t)217 || x == (seq_t)352)
            return 1;

        if (x <= (seq_t)20000) {
            int idx = (int)x;
            if (visited_mark[idx] == id)
                return 0;
            visited_mark[idx] = id;
        }
        x = sum_of_cubes(x);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */

    /* static state for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;
    static idx_t next_candidate = 1;

    /* generate terms until we have n of them */
    while (terms_cnt < n) {
        /* find the next integer congruent to 1 modulo 3 */
        while (next_candidate % 3 != 1)
            ++next_candidate;

        seq_t cand = (seq_t)next_candidate;
        if (reaches_cycle(cand)) {
            /* grow storage if required */
            if (terms_cnt >= terms_cap) {
                size_t newcap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) exit(1);  /* allocation failure */
                terms = tmp;
                terms_cap = newcap;
            }
            terms[terms_cnt++] = cand;
        }
        ++next_candidate;   /* move to next candidate for future searches */
    }

    return terms[n - 1];
}
