/*
 * A050822  Odd numbers in Hofstadter Q-sequence a(n)=a(n-a(n-1))+a(n-a(n-2)).
 * https://oeis.org/A050822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n‑th odd term of the Hofstadter Q sequence. Hofstadter Q:
 *            Q(1)=Q(2)=1, Q(k)=Q(k‑Q(k‑1))+Q(k‑Q(k‑2)) for k>2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *q_arr   = NULL;   /* full Hofstadter Q sequence (1‑based)   */
static size_t q_len   = 0;      /* number of computed Q terms              */
static size_t q_cap   = 0;      /* capacity of q_arr                       */
static seq_t *odd_arr = NULL;   /* odd subsequence of Q                    */
static size_t odd_len = 0;      /* number of computed odd terms           */
static size_t odd_cap = 0;      /* capacity of odd_arr                    */

/* Allocate / grow an array of seq_t, returning the new array pointer. */
static void ensure_q_cap(size_t need)
{
    if (need <= q_cap)
        return;
    size_t new_cap = q_cap ? q_cap << 1 : 8;
    if (new_cap < need)
        new_cap = need;
    q_arr = (seq_t *)realloc(q_arr, new_cap * sizeof(seq_t));
    q_cap = new_cap;
}

static void ensure_odd_cap(size_t need)
{
    if (need <= odd_cap)
        return;
    size_t new_cap = odd_cap ? odd_cap << 1 : 8;
    if (new_cap < need)
        new_cap = need;
    odd_arr = (seq_t *)realloc(odd_arr, new_cap * sizeof(seq_t));
    odd_cap = new_cap;
}

/* Initialise the static arrays on the first call. */
static void init(void)
{
    if (q_len > 0)          /* already initialised */
        return;

    /* initialise the Hofstadter Q sequence */
    ensure_q_cap(2);
    q_arr[0] = 1;           /* Q(1) */
    q_arr[1] = 1;           /* Q(2) */
    q_len   = 2;

    /* initialise the odd subsequence: first two odd terms are both 1 */
    ensure_odd_cap(2);
    odd_arr[0] = 1;
    odd_arr[1] = 1;
    odd_len   = 2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* n>=1 in the specification; 0 used only as safety */

    init();

    /* grow the sequences until we have at least n odd terms */
    while (odd_len < (size_t)n) {
        /* next index for the Hofstadter Q sequence (1‑based) */
        idx_t next_q = (idx_t)q_len + 1ULL;

        /* values of Q(n‑1) and Q(n‑2) already computed */
        seq_t a_nm1 = q_arr[q_len - 1];
        seq_t a_nm2 = q_arr[q_len - 2];

        /* compute the two indices required by the recurrence */
        idx_t idx1 = next_q - (idx_t)a_nm1 - 1ULL;   /* 0‑based index */
        idx_t idx2 = next_q - (idx_t)a_nm2 - 1ULL;

        /* safety: indices must be within [0, q_len-1] */
        size_t i1 = (size_t)idx1;
        size_t i2 = (size_t)idx2;

        seq_t val = q_arr[i1] + q_arr[i2];

        /* append to the full Q sequence */
        ensure_q_cap(q_len + 1);
        q_arr[q_len] = val;
        q_len++;

        /* if the value is odd, store it in the odd subsequence */
        if ((val % ((seq_t)2)) != 0) {
            ensure_odd_cap(odd_len + 1);
            odd_arr[odd_len] = val;
            odd_len++;
        }
    }

    return odd_arr[(size_t)n - 1];
}
