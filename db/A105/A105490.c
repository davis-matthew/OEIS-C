/*
 * A105490  Number of partitions of {1...n} containing 4 detached pairs of
 *          consecutive integers, i.e., partitions in which only 1- or
 *          2-strings of consecutive integers can appear in a block and there
 *          are exactly four 2-strings.
 * https://oeis.org/A105490
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n-4,4) * Bell(n-5) (for n >= 8)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t bell_max = 0;        /* highest n for which Bell has been computed */
static seq_t *bell_cache = NULL; /* Bell numbers B[0..bell_max]   */
static seq_t *prev_row  = NULL;  /* triangle row n-1 used to build row n */
static seq_t *curr_row  = NULL;  /* temporary triangle row */
static idx_t  alloc_cap = 0;     /* current allocation capacity */

/* Ensure that Bell numbers up to n are computed and stored in bell_cache */
static void ensure_bell(idx_t n)
{
    /* Allocate more space if needed */
    if (n >= alloc_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_bell = (seq_t*)realloc(bell_cache, new_cap * sizeof(seq_t));
        seq_t *new_prev = (seq_t*)realloc(prev_row,  new_cap * sizeof(seq_t));
        seq_t *new_curr = (seq_t*)realloc(curr_row,  new_cap * sizeof(seq_t));
        if (!new_bell || !new_prev || !new_curr) exit(1);
        bell_cache = new_bell;
        prev_row   = new_prev;
        curr_row   = new_curr;
        alloc_cap  = new_cap;
    }

    /* If nothing has been computed yet, initialise the first row */
    if (bell_max == 0) {
        bell_cache[0] = (seq_t)1;
        prev_row[0]   = (seq_t)1;
        bell_max      = 0;
    }

    /* Compute any missing Bell numbers up to n */
    for (idx_t k = bell_max + 1; k <= n; ++k) {
        /* curr_row[0] = last element of previous row */
        curr_row[0] = prev_row[k-1];
        /* Build the rest of the row */
        for (idx_t j = 1; j <= k; ++j) {
            curr_row[j] = curr_row[j-1] + prev_row[j-1];
        }
        bell_cache[k] = curr_row[0];          /* B_k */
        /* Copy current row to prev_row for next iteration */
        for (idx_t j = 0; j <= k; ++j) {
            prev_row[j] = curr_row[j];
        }
    }
    bell_max = n;
}

/* Return Bell(n)  (Bell numbers are 0‑indexed) */
static seq_t bell_value(idx_t n)
{
    ensure_bell(n);
    return bell_cache[n];
}

/* Main function required by the interface */
seq_t a(idx_t n)
{
    if (n < 8) return (seq_t)0; /* behaviour below offset is irrelevant */

    /* B = Bell(n-5) */
    seq_t B = bell_value(n - 5);

    /* binomial coefficient C(n-4,4) = (n-4)(n-5)(n-6)(n-7)/24 */
    seq_t comb = (seq_t)(n - 4);
    comb *= (seq_t)(n - 5);
    comb *= (seq_t)(n - 6);
    comb *= (seq_t)(n - 7);
    comb /= (seq_t)24;

    return comb * B;
}
