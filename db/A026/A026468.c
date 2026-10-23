/*
 * A026468  a(1) = 1, a(2) = 2; for n >= 3, a(n) = least positive integer >
 *          a(n-1) and not a(i)^2 + a(j)^2 for 1<=i<=j<=n-1.
 * https://oeis.org/A026468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(1)=1, a(2)=2; for n>=3, a(n)=least integer > a(n-1)
   not representable as a(i)^2 + a(j)^2 (1 ≤ i ≤ j ≤ n-1). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static storage for the sequence and the squares of its terms. */
static seq_t *a_seq   = NULL;   /* a(i) 0‑based indices */
static seq_t *sq_seq  = NULL;   /* a(i)^2                */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* current allocated capacity */

/* Ensure arrays can hold at least min_cap elements.  */
static void grow(size_t min_cap)
{
    if (seq_cap >= min_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    a_seq  = (seq_t *)realloc(a_seq,  new_cap * sizeof(seq_t));
    sq_seq = (seq_t *)realloc(sq_seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Check whether value can be expressed as sum of squares of previous terms.
   sq_seq[0..count-1] is sorted increasingly. */
static bool is_representable(seq_t value, size_t count)
{
    size_t i = 0;
    size_t j = count ? count - 1 : 0;
    while (i <= j) {
        seq_t sum = sq_seq[i] + sq_seq[j];
        if (sum == value)
            return true;
        if (sum < value)
            ++i;
        else
            --j;
    }
    return false;
}

/* Ensure that the sequence has at least n terms stored.  n is 1‑based. */
static void ensure_terms(idx_t n)
{
    for (idx_t idx = seq_len + 1; idx <= n; ++idx) {
        seq_t next;
        if (seq_len == 0)               /* a(1) */
            next = 1;
        else if (seq_len == 1)          /* a(2) */
            next = 2;
        else {
            seq_t candidate = a_seq[seq_len - 1] + 1;   /* > previous term */
            while (true) {
                if (!is_representable(candidate, seq_len)) {
                    next = candidate;
                    break;
                }
                ++candidate;
            }
        }
        grow(seq_len + 1);
        a_seq[seq_len]  = next;
        sq_seq[seq_len] = next * next;
        ++seq_len;
    }
}

/* External function requested by the specification. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* undefined, guard against misuse */
    ensure_terms(n);
    return a_seq[(size_t)(n - 1)];
}
