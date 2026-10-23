/*
 * A133137  a(1) = 1, a(2) = 2, a(n) = smallest number not the sum of 4th
 *          powers of 2 distinct earlier terms.
 * https://oeis.org/A133137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest integer not equal to a(i)^4 + a(j)^4 for all 1 ≤
 *            i < j < n; with a(1)=1, a(2)=2
 * Range:     u8 n <= 15, u16 n <= 16, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */
#include <stdlib.h>   /* for abort */

/* A safe upper bound for the required number of terms for n ≤ 10000. */
#define MAX_CAP 20000

/* static storage for the sequence and its fourth powers */
static seq_t seq[MAX_CAP];
static seq_t pow4[MAX_CAP];
static size_t seq_len = 0;

/* add a new term to the static arrays */
static void add_term(seq_t val)
{
    if (seq_len >= MAX_CAP)
        abort();                 /* should not happen for n ≤ 10000 */
    seq[seq_len] = val;
    seq_t p = val;
    p = p * p;   /* square */
    p = p * p;   /* fourth power */
    pow4[seq_len] = p;
    seq_len++;
}

/* test whether candidate can be written as a sum of two distinct earlier
   fourth powers using a two‑pointer search on the sorted array pow4. */
static int is_representable(seq_t candidate)
{
    size_t i = 0;
    size_t j = seq_len;
    if (j == 0) return 0;
    j--;                     /* last valid index */
    while (i < j) {
        seq_t sum = pow4[i] + pow4[j];
        if (sum == candidate) return 1;
        if (sum < candidate)
            i++;
        else
            j--;
    }
    return 0;
}

/* public interface required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    /* initialise the first two terms once */
    if (seq_len < 2) {
        add_term((seq_t)1);
        add_term((seq_t)2);
    }

    /* generate terms until we have at least n of them */
    seq_t candidate = seq[seq_len - 1] + (seq_t)1;   /* next integer to test */
    while ((size_t)n > seq_len) {
        if (!is_representable(candidate))
            add_term(candidate);
        candidate++;
    }

    return seq[(size_t)n - 1];
}
