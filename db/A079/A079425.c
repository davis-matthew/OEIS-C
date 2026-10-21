/*
 * A079425  Marks on lexicographically earliest 9-mark optimal Golomb ruler.
 * https://oeis.org/A079425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: exhaustive DFS to find the lexicographically earliest
   9‑mark optimal Golomb ruler.  The marks are returned by a(idx_t n),
   with a(1)=0, a(2)=1, …, a(9)=44.  */

/* The function below computes the marks by searching for all
   9‑mark Golomb rulers, keeping the one with minimal length and
   lexicographically earliest.  It uses only integer arithmetic. */

#include <stdbool.h>
#include <stddef.h>

static seq_t mark[9];       /* current construction */
static seq_t best[9];       /* best solution found so far         */
static seq_t best_len;     /* length of best solution            */
static bool best_found;    /* whether best_len has been set      */
static bool computed;      /* whether the search has completed   */
static bool used[512];     /* used differences, indexed by diff  */

/* recursive depth‑first search for positions of the marks
   starting from position 'pos' (0‑based, current last mark is 'last') */
static void dfs(int pos, seq_t last)
{
    if (pos == 9) {
        if (!best_found || last < best_len) {
            best_found = true;
            best_len = last;
            for (int i = 0; i < 9; ++i) best[i] = mark[i];
        }
        return;
    }

    int rem = 9 - pos; /* remaining marks to place (including current one) */

    /* prune if even placing smallest possible marks exceeds known best */
    if (best_found) {
        seq_t lower_bound = last + (seq_t)rem;
        if (lower_bound > best_len) return;
    }

    /* upper bound for next candidate */
    seq_t upper_seq;
    if (best_found) {
        upper_seq = best_len - (seq_t)rem;
    } else {
        upper_seq = (seq_t)200;          /* safe upper bound for search */
    }
    if (upper_seq < last + (seq_t)1) return;

    for (seq_t cand = last + (seq_t)1; cand <= upper_seq; ++cand) {
        bool conflict = false;

        /* check whether candidate introduces a repeated difference */
        for (int i = 0; i < pos; ++i) {
            seq_t d = cand - mark[i];
            if (used[(idx_t)d]) { conflict = true; break; }
        }
        if (conflict) continue;

        /* mark new differences */
        for (int i = 0; i < pos; ++i) {
            seq_t d = cand - mark[i];
            used[(idx_t)d] = true;
        }

        mark[pos] = cand;
        dfs(pos + 1, cand);

        /* unmark differences for backtracking */
        for (int i = 0; i < pos; ++i) {
            seq_t d = cand - mark[i];
            used[(idx_t)d] = false;
        }

        /* further candidates will be larger */
        if (best_found) {
            seq_t tentative = cand + (seq_t)rem;
            if (tentative > best_len) break;
        }
    }
}

/* run the search once */
static void compute(void)
{
    if (computed) return;
    for (int i = 0; i < 512; ++i) used[i] = false;
    mark[0] = 0;
    best_found = false;
    dfs(1, 0);
    computed = true;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 9) return 0;
    if (!computed) compute();
    return best[(size_t)(n - 1)];
}
