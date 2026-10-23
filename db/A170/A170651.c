/*
 * A170651  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A170651: Number of reduced words of length n in the Coxeter group
   on 18 generators with relations (S_i)^2 = (S_i S_j)^49 = I.
   The generating function is 
        P(t) / Q(t)   with
           P(t) = 1 + 2*t + 2*t^2 + ... + 2*t^48 + t^49 ,
           Q(t) = 1 - 16*t - 16*t^2 - ... - 16*t^48 + 136*t^49 .
   Hence for n >= 50 the terms satisfy the linear recurrence
        a(n) = 16*(a(n-1)+...+a(n-48)) - 136*a(n-49) ,
   with the following inhomogeneous formulas for the first 49 terms:
        a(0) = 1,
        a(n) = 16 * (a(0)+...+a(n-1)) + 2      for 1 <= n <= 48,
        a(49) = 16 * (a(1)+...+a(48)) - 136*a(0) + 1 .
   This implementation memoises previously computed terms and uses the
   recurrence to compute any requested value efficiently.
*/

#include <stdlib.h>

static seq_t *seq      = NULL;   /* sequence values a(0), a(1), ... */
static idx_t  seq_len  = 0;      /* number of computed terms */
static idx_t  seq_cap  = 0;      /* allocated capacity for seq */

static seq_t *prefix   = NULL;   /* prefix sums: prefix[i] = sum_{k=0}^{i-1} a(k) */
static idx_t  prefix_len = 0;    /* number of prefix entries, should be seq_len+1 */
static idx_t  prefix_cap = 0;    /* allocated capacity for prefix */

/* Ensure capacity for seq array (grow until >= need). */
static void ensure_seq_capacity(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 1;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* memory failure */
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure capacity for prefix array (grow until >= need). */
static void ensure_prefix_capacity(idx_t need)
{
    if (need <= prefix_cap) return;
    idx_t new_cap = prefix_cap ? prefix_cap * 2 : 2;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prefix, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    prefix = tmp;
    prefix_cap = new_cap;
}

/* Initialise the memoisation structures with a(0)=1.  */
static void init_sequence(void)
{
    if (seq) return;          /* already initialised */
    seq_cap = 1;              /* allocate one slot for a(0) */
    seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq) abort();
    seq[0] = (seq_t)1;

    prefix_cap = 2;           /* prefix[0]=0, prefix[1]=a(0) */
    prefix = (seq_t *)malloc(prefix_cap * sizeof(seq_t));
    if (!prefix) abort();
    prefix[0] = (seq_t)0;
    prefix[1] = (seq_t)1;

    seq_len   = 1;
    prefix_len = 2;
}

/* Return a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    init_sequence();

    /* Expand the memoised sequence up to n if necessary. */
    while (seq_len <= n) {
        idx_t k = seq_len;                /* index of term to compute */
        seq_t term;

        if (k <= 48) {                 /* 1 <= k <= 48 */
            seq_t p = (k == 0) ? (seq_t)1 : (seq_t)2;
            term = (seq_t)16 * prefix[k] + p;
        }
        else if (k == 49) {              /* special inhomogeneous term */
            seq_t sum_1_to_48 = prefix[49] - seq[0];
            term = (seq_t)16 * sum_1_to_48 - (seq_t)136 * seq[0] + (seq_t)1;
        }
        else {                          /* homogeneous recurrence for k >= 50 */
            seq_t sum_last48 = prefix[k] - prefix[k-49];
            term = (seq_t)16 * sum_last48 - (seq_t)136 * seq[k-49];
        }

        /* Grow storage if required. */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = term;

        ensure_prefix_capacity(prefix_len + 1);
        prefix[prefix_len] = prefix[prefix_len - 1] + term;

        seq_len++;
        prefix_len++;
    }

    return seq[n];
}
