/*
 * A076428  Numbers k such that the sum of digits of k in superfactorial base
 *          divides k.
 * https://oeis.org/A076428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n‑th integer k such that the sum of the digits of k in
 *            the superfactorial base divides k
 * Range:     u8 n <= 49, u16 n <= 1503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- Superfactorial weights (A000178) ---------------------------------- */
#include <stddef.h>     /* for size_t */

/* maximum number of terms to remember; sufficient for n up to ~10 000 */
#define MAXSEQ 20000

/* static tables used by the routine */
static seq_t sf[200];            /* superfactorial weights S_1 .. */
static idx_t sf_len = 0;         /* number of weights stored            */
static idx_t sf_next_i = 2;      /* next factorial index to compute (starts at 2!) */
static seq_t sf_next_fact = 1;   /* factorial of (sf_next_i-1), i.e. (i-1)! */

/* Ensure that all weights ≤ limit have been generated.  The routine
   stops when further computation would overflow the seq_t type. */
static void ensure_weights_for(seq_t limit)
{
    if (sf_len == 0) {
        sf[0] = 1;            /* S₁ = 1! = 1 */
        sf_len = 1;
        sf_next_i = 2;        /* next factorial to compute is 2! */
        sf_next_fact = 1;     /* factorial of (2-1)! = 1! = 1 */
    }

    while (sf[sf_len-1] <= limit) {
        /* compute next factorial: sf_next_fact *= sf_next_i */
        seq_t new_fact = sf_next_fact * sf_next_i;
        if (sf_next_i != 0 && new_fact / sf_next_i != sf_next_fact)
            break;                 /* multiplication overflowed */

        /* compute next weight:  new_weight = last_weight * new_fact */
        seq_t last_weight = sf[sf_len-1];
        seq_t new_weight = last_weight * new_fact;
        if (new_fact != 0 && new_weight / new_fact != last_weight)
            break;                 /* multiplication overflowed */

        if (sf_len >= sizeof(sf)/sizeof(sf[0])) break;  /* table full */

        sf[sf_len++] = new_weight;
        sf_next_fact = new_fact;
        ++sf_next_i;
    }
}

/* Sum of the digits of x in the superfactorial base. */
static seq_t super_sf_digit_sum(seq_t x)
{
    if (x == 0) return 0;

    ensure_weights_for(x);

    seq_t sum = 0;
    for (idx_t i = sf_len; i > 0; --i) {
        seq_t w = sf[i-1];
        if (w > x) continue;
        seq_t d = x / w;
        sum += d;
        x %= w;
        if (x == 0) break;
    }
    return sum;
}

/* ---- Sequence storage ---------------------------------------------------- */
static seq_t seq_term[MAXSEQ];
static size_t seq_len = 0;      /* how many terms have been generated */
static seq_t last_checked = 0;  /* last integer examined               */

/* ---- Main entry point --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1: a(1) is the first term */

    /* Generate terms up to the requested index. */
    while (seq_len < (size_t)n) {
        ++last_checked;                 /* examine the next integer */

        seq_t dsum = super_sf_digit_sum(last_checked);
        if (dsum != 0 && last_checked % dsum == 0) {
            if (seq_len < (size_t)MAXSEQ) {
                seq_term[seq_len++] = last_checked;
            } else {
                /* buffer overflow – should not happen for n<=10 000 */
                break;
            }
        }
    }

    return seq_term[n-1];
}
