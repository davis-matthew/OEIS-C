/*
 * A026447  a(n) = least positive integer > a(n-1) and not a(i)*a(j)+2 for
 *          1<=i<j<=n, with initial terms 3,4.
 * https://oeis.org/A026447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A026447
   By construction:
   a(1)=3, a(2)=4.
   For n>2, a(n) is the smallest integer > a(n-1) that is NOT of the
   form a(i)*a(j)+2 for any 1 <= i < j <= n.
   This code builds the sequence incrementally and memoises all terms
   that have been computed.  The sequence is strictly increasing,
   so a binary search over the already‑computed terms is sufficient
   for the membership test.
*/

#include <stdlib.h>
#include <stdbool.h>

/* Binary search to test whether val occurs in the sorted array a[0..len-1]. */
static bool seq_contains(const seq_t *a, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (a[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < len && a[lo] == val);
}

/* Grow the dynamic array to at least new_len elements. */
static void ensure_capacity(size_t new_len, seq_t **arr, size_t *cap)
{
    if (new_len <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort to avoid undefined behaviour. */
        abort();
    }
    *arr = tmp;
    *cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    if (n == 0)
        return 0;                /* undefined for n < 1; return 0 as a guard */

    /* Grow the sequence up to the requested index n. */
    while (seq_len < (size_t)n) {
        /* Initialise the first two terms once. */
        if (seq_len == 0) {
            ensure_capacity(1, &seq, &seq_cap);
            seq[0] = (seq_t)3;
            seq_len = 1;
            continue;
        }
        if (seq_len == 1) {
            ensure_capacity(2, &seq, &seq_cap);
            seq[1] = (seq_t)4;
            seq_len = 2;
            continue;
        }

        /* Find the next admissible candidate. */
        seq_t cand = seq[seq_len - 1] + (seq_t)1;   /* > previous term */
        for (;;) {
            seq_t t = cand - (seq_t)2;
            bool forbidden = false;
            for (size_t i = 0; i < seq_len && !forbidden; ++i) {
                seq_t ai = seq[i];
                /* Prevent overflow: if ai*ai > t continue. */
                if (ai > t / ai)
                    continue;
                if (t % ai == 0) {
                    seq_t q = t / ai;
                    if (q > ai && seq_contains(seq, seq_len, q)) {
                        forbidden = true;
                        break;
                    }
                }
            }
            if (!forbidden)
                break;           /* cand is acceptable */
            ++cand;              /* try next integer */
        }

        /* Append the accepted candidate to the sequence. */
        ensure_capacity(seq_len + 1, &seq, &seq_cap);
        seq[seq_len++] = cand;
    }

    return seq[(size_t)n - 1];
}
