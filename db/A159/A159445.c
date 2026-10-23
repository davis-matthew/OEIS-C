/*
 * A159445  Number of n X n arrays of squares of integers summing to 40.
 * https://oeis.org/A159445
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of n×n arrays of integer squares summing to 40.
   Enumerate all ways to write 40 as Σ w_i·c_i over squares
   w_i ∈ {1,4,9,16,25,36}, then for each solution add the
   multinomial coefficient selecting the positions of the squares
   among the n² cells. */
#include <stddef.h>

/* Representation of a solution: how many cells contain 1, 4, 9, 16, 25 and 36. */
struct Solution {
    int c1, c4, c9, c16, c25, c36;
};

#define MAX_SOL 64
static struct Solution solutions[MAX_SOL];
static size_t sol_cnt = 0;

/* Recursively enumerate all solutions to
   1·c1 + 4·c4 + 9·c9 + 16·c16 + 25·c25 + 36·c36 = 40. */
static void build_solutions(int pos, int remaining, int *cnt)
{
    if (pos == 6) {
        if (remaining == 0 && sol_cnt < MAX_SOL) {
            solutions[sol_cnt].c1  = cnt[0];
            solutions[sol_cnt].c4  = cnt[1];
            solutions[sol_cnt].c9  = cnt[2];
            solutions[sol_cnt].c16 = cnt[3];
            solutions[sol_cnt].c25 = cnt[4];
            solutions[sol_cnt].c36 = cnt[5];
            ++sol_cnt;
        }
        return;
    }
    static const int sq[6] = {1, 4, 9, 16, 25, 36};
    int maxc = remaining / sq[pos];
    for (int c = 0; c <= maxc; ++c) {
        cnt[pos] = c;
        build_solutions(pos + 1, remaining - c * sq[pos], cnt);
    }
}

/* Binomial coefficient C(n, k) with n a seq_t and k a small idx_t. */
static seq_t binom(seq_t n, idx_t k)
{
    if (k == 0) return (seq_t)1;
    if (n < (seq_t)k) return (seq_t)0;   /* n < k → 0 */

    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i) {
        res = res * (n - (seq_t)i);   /* numerator term n-i */
        res = res / (i + 1);           /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* below the offset */

    /* Build the list of solutions once. */
    if (sol_cnt == 0) {
        int cnt[6] = {0};
        build_solutions(0, 40, cnt);
    }

    seq_t n2 = (seq_t)n * (seq_t)n;          /* number of cells */
    seq_t result = 0;

    for (size_t i = 0; i < sol_cnt; ++i) {
        const struct Solution *s = &solutions[i];
        seq_t nonzero = (seq_t)s->c1 + s->c4 + s->c9 + s->c16 + s->c25 + s->c36;
        if (nonzero > n2) continue;          /* can't fit into the array */

        seq_t rem = n2;
        seq_t ways = 1;

        ways = ways * binom(rem, (idx_t)s->c1);  rem -= (seq_t)s->c1;
        ways = ways * binom(rem, (idx_t)s->c4);  rem -= (seq_t)s->c4;
        ways = ways * binom(rem, (idx_t)s->c9);  rem -= (seq_t)s->c9;
        ways = ways * binom(rem, (idx_t)s->c16); rem -= (seq_t)s->c16;
        ways = ways * binom(rem, (idx_t)s->c25); rem -= (seq_t)s->c25;
        ways = ways * binom(rem, (idx_t)s->c36); rem -= (seq_t)s->c36;
        /* remaining cells are zeros; no further factor is needed */

        result += ways;
    }

    return result;
}
