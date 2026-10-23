/*
 * A041204  Numerators of continued fraction convergents to sqrt(113).
 * https://oeis.org/A041204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A041204 – Numerators of continued‑fraction convergents to sqrt(113)
 *
 *  Recurrence:
 *      a(0) = ⌊√113⌋ = 10
 *      Let (m_k, d_k, a_k) be the standard CF data for √113,
 *      with (m_0,d_0,a_0) = (0,1,10).  Then for k ≥ 0
 *          m_{k+1} = d_k*a_k – m_k
 *          d_{k+1} = (113 – m_{k+1}²) / d_k
 *          a_{k+1} = ⌊(10 + m_{k+1}) / d_{k+1}⌋
 *
 *      The numerators satisfy
 *          p_{-1} = 1,   p_0 = a_0
 *          p_{k+1} = a_{k+1} * p_k + p_{k-1}
 *
 *      This function returns p_n.
 *
 *  The implementation lazily generates terms on demand and memoises
 *  them in a dynamically allocated array.  No external utilities are
 *  required; only standard library functions are used.
 *  All arithmetic is performed on the type seq_t, which may be any
 *  unsigned integer or an arbitrary‑precision integer class, as
 *  required by the build system.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t */

static seq_t *memo = NULL;   /* memoised numerators */
static size_t    memo_cap = 0; /* allocated capacity */
static size_t    memo_len = 0; /* number of stored terms */

/* State of the continued fraction generation */
static uint64_t cf_m = 0;   /* current m_k   */
static uint64_t cf_d = 1;   /* current d_k   */
static uint64_t cf_a = 10;  /* current a_k   */
static const uint64_t CF_D = 113ULL;      /* the value under the square root */
static const uint64_t CF_A0 = 10ULL;      /* floor(√113) */

/* Initialise memoisation array and CF state */
static void
init_state(void)
{
    if (memo != NULL) return;

    memo_cap = 8;                    /* initial capacity */
    memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    if (!memo) abort();

    memo[0] = (seq_t)CF_A0;          /* p_0 = a_0 */
    memo_len = 1;                    /* one term stored */
    cf_m = 0;
    cf_d = 1;
    cf_a = CF_A0;
}

/* Grow the memoisation array to hold at least 'need' elements */
static void
ensure_capacity(size_t need)
{
    if (need < memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 8;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    memo = tmp;
    memo_cap = new_cap;
}

/* Return the n‑th term of the sequence.  n is an unsigned index. */
seq_t
a(idx_t n)
{
    init_state();

    size_t idx = (size_t)n;
    if (idx < memo_len) return memo[idx];

    /* Generate terms up to index n */
    while (memo_len <= idx) {
        /* Compute next continued‑fraction term a_{next} */
        uint64_t m_next = cf_d * cf_a - cf_m;
        uint64_t d_next = (CF_D - m_next * m_next) / cf_d;
        uint64_t a_next = (CF_A0 + m_next) / d_next;

        /* Compute next numerator p_{next} = a_next * p_k + p_{k-1} */
        seq_t prev      = memo[memo_len - 1];
        seq_t prev_prev = (memo_len >= 2) ? memo[memo_len - 2] : (seq_t)1;
        seq_t next_num  = (seq_t)a_next * prev + prev_prev;

        /* Store the new value, expanding storage if necessary */
        ensure_capacity(memo_len + 1);
        memo[memo_len] = next_num;
        memo_len++;

        /* Advance the CF state to the next partial quotient */
        cf_m = m_next;
        cf_d = d_next;
        cf_a = a_next;
    }

    return memo[idx];
}
