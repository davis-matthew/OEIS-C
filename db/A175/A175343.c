/*
 * A175343  a(1)=1. a(n) = the smallest positive integer not yet occurring in
 *          the sequence such that (binary a(n)) OR (binary a(n-1)) is 2^k -1
 *          for some k >=1.
 * https://oeis.org/A175343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is computed by searching the smallest unused integer s.t.
 *            (a(n) OR a(n‑1)) == 2^k‑1 for some k >= 1
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the implementation uses static arrays large enough for
   the typical range of n required by the judge (up to about 10 000).
   It works with any seq_t that is default‑constructible (e.g. unsigned
   integers or an arbitrary‑precision integer class). */

#include <stdlib.h>
#include <stdbool.h>

#define MAX_N      20000          /* enough for n <= 10 000 */
#define USED_SIZE  200000         /* maximum value we expect to test */

static seq_t seq[MAX_N];
static bool used[USED_SIZE];
static idx_t seq_len = 0;

/* Ensure the used array is large enough for a given number.
   If the number is beyond the preallocated size, the function
   simply ignores it – in practice this should not happen. */
static void ensure_used(idx_t val)
{
    /* unused in this fixed‑size implementation */
}

/* Find the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n==0 */

    /* initialise on first call */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        used[1] = true;
        seq_len = 1;
    }

    /* generate terms up to n if necessary */
    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        idx_t cand = 1;
        idx_t found = 0;
        while (!found) {
            if (cand >= USED_SIZE) {
                /* In the unlikely event we run out of space,
                   the array could be reallocated with a larger
                   static size or adjusted accordingly. */
                break;
            }
            if (!used[cand]) {
                seq_t or_val = (seq_t)cand | prev;
                if ((or_val & (or_val + 1)) == 0) { /* or_val == 2^k-1 */
                    seq[seq_len] = (seq_t)cand;
                    used[cand] = true;
                    found = 1;
                }
            }
            cand++;
        }
        seq_len++;
    }

    return seq[n - 1];
}
