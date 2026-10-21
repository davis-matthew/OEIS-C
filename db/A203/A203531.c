/*
 * A203531  Run lengths in Golay-Rudin-Shapiro sequence A020985.
 * https://oeis.org/A203531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = length of the n‑th run of 1s, a(2*n+1) = length of the
 *            n‑th run of –1s in the Golay–Rudin–Shapiro sequence, defined by
 *            s(n)=(-1)^{popcount(n&(n>>1))}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   parity_of_popcount_shift, sign_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc, abort */
#include "../utils/parity_of_popcount_shift.h"
#include "../utils/sign_of.h"

static seq_t *run_lengths   = NULL;
static size_t  runs_cap     = 0;
static size_t  runs_size    = 0;          /* number of run lengths stored */
static int     last_sign    = 0;          /* 1 for +1, -1 for -1, 0 means uninitialized */
static size_t  current_len  = 0;          /* length of the current run */
static idx_t   next_index   = 0;          /* next index of the sign sequence to process */

/* ensure run_lengths has capacity for at least min_cap elements */
static void grow_runs(size_t min_cap)
{
    if (runs_cap >= min_cap)
        return;
    size_t new_cap = runs_cap ? runs_cap << 1 : 4;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(run_lengths, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* allocation failed */
    run_lengths = tmp;
    runs_cap = new_cap;
}

/* generate runs until runs_size > target (0‑based index) */
static void ensure_runs_up_to(idx_t target)
{
    while (runs_size <= target) {
        int s = sign_of(next_index++);
        if (last_sign == 0) {             /* first element of the sequence */
            last_sign = s;
            current_len = 1;
        } else if (s == last_sign) {
            current_len++;
        } else {                          /* run finished */
            grow_runs(runs_size + 1);
            run_lengths[runs_size++] = (seq_t)current_len;
            last_sign = s;
            current_len = 1;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_runs_up_to(n);
    return run_lengths[n];
}
