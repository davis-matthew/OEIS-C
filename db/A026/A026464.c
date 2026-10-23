/*
 * A026464  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-2 for
 *          1<=i<=j<=n.
 * https://oeis.org/A026464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer > a(n-1) not equal to a(i)*a(j)-2
 *            * for all 1 ≤ i ≤ j ≤ n. Indices start at 1 (a(1) = 3). The
 *            sequence is * memoised so calls may arrive in any order
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*  --- global memoisation helpers ---------------------------------- */
static seq_t *seq = NULL;   /* 1‑based array of computed terms */
static idx_t seq_len = 0;   /* highest index stored so far */
static size_t seq_cap = 0;  /* allocated capacity (in terms) */

/*  --- grow the array dynamically ----------------------------------- */
static void grow_seq(void)
{
    size_t new_cap = (seq_cap == 0) ? 2 : seq_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (new_ptr == NULL) exit(1);      /* allocation failed */
    seq = new_ptr;
    seq_cap = new_cap;
}

/*  --- initialise with the first term -------------------------------- */
static void seq_init(void)
{
    if (seq_len != 0) return;  /* already initialised */

    seq_cap = 2;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (seq == NULL) exit(1);   /* allocation failed */
    seq[1] = (seq_t)3;
    seq_len = 1;
}

/*  --- binary search in seq[ start .. end ] (inclusive) -------------- */
static bool seq_contains(idx_t start, idx_t end, seq_t q)
{
    idx_t lo = start;
    idx_t hi = end;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (seq[mid] < q) {
            lo = mid + 1;
        } else if (seq[mid] > q) {
            hi = mid - 1;
        } else {
            return true;
        }
    }
    return false;
}

/*  --- test whether a candidate is forbidden ------------------------ */
static bool is_forbidden(seq_t candidate, idx_t cur_len)
{
    seq_t sum = candidate + (seq_t)2;          /* candidate+2 */
    for (idx_t i = 1; i <= cur_len; ++i) {
        seq_t ai = seq[i];
        if (sum % ai != 0) continue;         /* not divisible */
        seq_t q = sum / ai;
        if (q < ai) break;                   /* later i give smaller q */
        if (seq_contains(i, cur_len, q))
            return true;
    }
    return false;
}

/*  --- public accessor ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for 0 or negative indices */
    seq_init();

    /* compute terms up to n if needed */
    while (seq_len < n) {
        /* next integer after the previous term */
        seq_t candidate = seq[seq_len] + (seq_t)1;
        while (is_forbidden(candidate, seq_len))
            candidate = candidate + (seq_t)1;

        /* ensure capacity for the new term */
        if (seq_len + 1 >= seq_cap)
            grow_seq();

        /* store the new term */
        ++seq_len;
        seq[seq_len] = candidate;
    }
    return seq[n];
}
