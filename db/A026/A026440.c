/*
 * A026440  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for 1
 *          <= i <= j < n, n >= 2, with initial terms 1,4.
 * https://oeis.org/A026440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A026440: a(1) = 1, a(2) = 4.
   For n ≥ 3, a(n) is the smallest positive integer > a(n‑1) that is
   NOT of the form a(i)*a(j) – 1 for any 1 ≤ i ≤ j < n.
*/
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequence */
static seq_t *seq_vals = NULL;      /* array of computed terms, sorted */
static size_t seq_len  = 0;         /* number of terms stored */
static size_t seq_cap  = 0;         /* allocated capacity */

/* allocate or grow the array to accommodate at least 'min_cap' entries */
static void ensure_capacity(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* binary search for a value in seq_vals[0 .. seq_len-1] */
static bool contains(seq_t value)
{
    size_t lo = 0, hi = seq_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seq_vals[mid] == value) return true;
        if (seq_vals[mid] < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* initialise with the two known terms */
static void init_sequence(void)
{
    if (seq_vals) return;
    ensure_capacity(8);
    seq_vals[0] = (seq_t)1;
    seq_vals[1] = (seq_t)4;
    seq_len = 2;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe return */
    init_sequence();

    /* extend sequence up to requested index */
    while (seq_len < (size_t)n) {
        seq_t prev = seq_vals[seq_len - 1];
        seq_t cand = prev + (seq_t)1;

        /* find next admissible candidate */
        for (;;) {
            seq_t sum = cand + (seq_t)1;     /* sum = a(i)*a(j) */
            bool forbidden = false;

            /* check all previous a[i] that could participate */
            for (size_t idx = 0; idx < seq_len; ++idx) {
                seq_t ai = seq_vals[idx];
                /* stop if ai*ai > sum */
                if (ai > (seq_t)0 && ai > sum / ai) break;
                if ((sum % ai) == (seq_t)0) {
                    seq_t bi = sum / ai;
                    if (contains(bi)) {
                        forbidden = true;
                        break;
                    }
                }
            }

            if (!forbidden) break;   /* cand is accepted */
            ++cand;
        }

        /* store the new term */
        ensure_capacity(seq_len + 1);
        seq_vals[seq_len++] = cand;
    }

    return seq_vals[(size_t)n - 1];
}
