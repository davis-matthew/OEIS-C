/*
 * A186362  Number of non-up-down cycles in all permutations of {1,2,...,n}. A
 *          cycle (b(1), b(2), ...) is said to be up-down if, when written
 *          with its smallest element in the first position, it satisfies
 *          b(1)<b(2)>b(3)<... .
 * https://oeis.org/A186362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{j=1..n} ((j-1)! - E(j-1)) / j! where E(k) are
 *            Euler up/down numbers (A000111). The recurrence
 *            a(n)=n*a(n-1)+(n-1)!-E(n-1) is used, with E(0)=E(1)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static state for memoization */
static seq_t *fact = NULL;
static seq_t *euler = NULL;
static seq_t *a_seq = NULL;
static idx_t size = 0;      /* capacity of the arrays */

/* ensure arrays are computed up to at least n */
static void ensure_up_to(idx_t n)
{
    if (size > n) return;

    idx_t new_cap = size == 0 ? 1 : size;
    while (new_cap <= n) new_cap = new_cap * 2 + 1;

    seq_t *new_fact = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *new_eul  = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *new_a   = (seq_t *)malloc(new_cap * sizeof(seq_t));

    if (size > 0) {
        memcpy(new_fact, fact, size * sizeof(seq_t));
        memcpy(new_eul,  euler, size * sizeof(seq_t));
        memcpy(new_a,    a_seq, size * sizeof(seq_t));
    }

    /* fill new entries */
    for (idx_t i = size; i < new_cap; ++i) {
        if (i == 0) {
            new_fact[0] = (seq_t)1;
            new_eul[0]  = (seq_t)1;
            new_a[0]    = (seq_t)0;
            continue;
        }

        /* factorial */
        new_fact[i] = new_fact[i-1] * (seq_t)i;

        /* Euler numbers */
        if (i == 1)
            new_eul[i] = (seq_t)1;
        else {
            seq_t binom = (seq_t)1;
            seq_t sum   = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                sum += binom * new_eul[k] * new_eul[i-1-k];
                if (k + 1 < i)
                    binom = binom * (seq_t)(i-1-k) / (seq_t)(k+1);
            }
            new_eul[i] = sum / (seq_t)2;
        }

        /* a(n) recurrence */
        new_a[i] = (seq_t)i * new_a[i-1] + new_fact[i-1] - new_eul[i-1];
    }

    /* replace old arrays */
    free(fact);
    free(euler);
    free(a_seq);
    fact  = new_fact;
    euler = new_eul;
    a_seq = new_a;
    size  = new_cap;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_seq[n];
}
