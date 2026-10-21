/*
 * A065375  The next smallest pair of numbers is taken so that a(2n-1)/a(2n)
 *          converges to sqrt(2).
 * https://oeis.org/A065375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065375:
   The sequence is obtained by merging, in increasing order of the sum x+y,
   the three infinite Pell sequences of solutions to
   x^2-2y^2 = -1,  x^2-2y^2 = +1,  x^2-2y^2 = -2.
   Each sequence satisfies the recurrence
        (x_{k+1},y_{k+1}) = (3x_k+4y_k , 2x_k+3y_k)
   with seeds (1,1), (3,2) and (4,3) respectively.  The list a(n) is
   obtained by taking the k‑th merged pair for k = ⌊(n-1)/2⌋ and
   outputting the first (odd n) or second (even n) component. */

#include <stdlib.h>
#include <stddef.h>

typedef struct { seq_t p; seq_t q; } Pair;

static Pair current[3];
static Pair *result = NULL;
static size_t result_len = 0;
static size_t result_cap = 0;
static size_t init_flag = 0;

/* Grow the result array to accommodate more pairs. */
static void grow_result(void)
{
    size_t new_cap = (result_cap == 0) ? 4 : result_cap << 1;
    result = (Pair*)realloc(result, new_cap * sizeof(Pair));
    result_cap = new_cap;
}

/* Generate the merged sequence up to 'need' pairs (0‑based count). */
static void ensure_pairs(size_t need)
{
    if (!init_flag) {                         /* first time only      */
        current[0].p = (seq_t)1; current[0].q = (seq_t)1;
        current[1].p = (seq_t)3; current[1].q = (seq_t)2;
        current[2].p = (seq_t)4; current[2].q = (seq_t)3;
        grow_result();                        /* initialise capacity */
        init_flag = 1;
    }

    while (result_len < need) {
        /* Select the sequence with the smallest sum. */
        int best = 0;
        seq_t best_sum = current[0].p + current[0].q;
        for (int j = 1; j < 3; ++j) {
            seq_t s = current[j].p + current[j].q;
            if (s < best_sum) {
                best_sum = s;
                best = j;
            }
        }

        /* Append this pair to the merged list. */
        if (result_len == result_cap) grow_result();
        result[result_len++] = current[best];

        /* Advance the chosen sequence using the recurrence. */
        Pair next;
        next.p = ((seq_t)3) * current[best].p + ((seq_t)4) * current[best].q;
        next.q = ((seq_t)2) * current[best].p + ((seq_t)3) * current[best].q;
        current[best] = next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for offset 1 */

    size_t pair_idx = ((size_t)n + 1) / 2 - 1;   /* 0‑based pair index */
    ensure_pairs(pair_idx + 1);

    const Pair *p = &result[pair_idx];
    return (n & 1ULL) ? p->p : p->q;
}
