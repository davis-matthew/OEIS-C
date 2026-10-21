/*
 * A235748  Ruler function associated with the set of permutations generated
 *          by cyclic shift, array read by rows.
 * https://oeis.org/A235748
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: E(2) = [1].
   For n>2, E(n) is obtained by inserting a run of n‑1 ones between
   all entries of E(n‑1) (after each entry) and at both extremities.
   The sequence a(n) is the concatenation of all E(k) for k≥2. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* maximum row needed for indices up to 10 000 (cum[8] = 46 225 > 10 000) */
#define MAX_R 25

static unsigned __int128 fact[MAX_R + 1];
static unsigned __int128 len_arr[MAX_R + 1];
static unsigned __int128 cum[MAX_R + 1];

/* initialise factorials, lengths and cumulative sums once */
static void init_arrays(void)
{
    static bool done = false;
    if (done) return;
    fact[0] = 1;
    for (idx_t i = 1; i <= MAX_R; ++i)
        fact[i] = fact[i-1] * (unsigned __int128)i;

    len_arr[0] = 0;
    len_arr[1] = 0;
    cum[0] = 0;
    cum[1] = 0;
    for (idx_t i = 2; i <= MAX_R; ++i) {
        len_arr[i] = fact[i] - 1;          /* |E(i)|   */
        cum[i]    = cum[i-1] + len_arr[i]; /* sum_{k=2}^i |E(k)| */
    }
    done = true;
}

/* recursive computation of a value inside E(row) at position pos (0‑based) */
static seq_t get_E(idx_t row, idx_t pos)
{
    if (row == 2) return 1;                 /* E(2) = [1] */

    idx_t run = row - 1;                    /* number of 1's inserted */
    unsigned __int128 blocksize = (unsigned __int128)(run + 1);
    unsigned __int128 len_prev  = len_arr[row-1];
    unsigned __int128 total_blocks = len_prev * blocksize;
    unsigned __int128 pos128 = (unsigned __int128)pos;

    if (pos128 < total_blocks) {
        unsigned __int128 block  = pos128 / blocksize;
        unsigned __int128 offset = pos128 % blocksize;
        if (offset < (unsigned __int128)run) {
            return 1;                       /* run of 1's */
        } else {
            /* element from E(row‑1), incremented by 1 */
            return get_E(row - 1, (idx_t)block) + 1;
        }
    } else {
        return 1;                           /* final run of 1's */
    }
}

/* public interface: a(n) for n ≥ 2 */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* undefined part of the sequence */

    init_arrays();                          /* prepare tables */

    idx_t offset = n - 2;                    /* zero‑based index in the concatenated rows */

    /* find which row contains this index */
    idx_t r = 2;
    for (; r <= MAX_R; ++r) {
        if ((unsigned __int128)offset < cum[r]) break;
    }
    if (r > MAX_R) return 0;                /* should not happen for n ≤ 10 000 */

    unsigned __int128 prev_cum = (r > 2 ? cum[r-1] : (unsigned __int128)0);
    idx_t pos = offset - (idx_t)prev_cum;    /* zero‑based index inside row r */

    return get_E(r, pos);
}
