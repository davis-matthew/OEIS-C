/*
 * A096849  If f(x) = phi(sigma(x)) is iterated starting from these numbers,
 *          then the start-value never returns. These are the transient terms
 *          of this iteration. Never occur in terminal cycles.
 * https://oeis.org/A096849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9336 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Transient terms under iteration of f(x)=φ(σ(x)).
   For a starting integer x the function f is applied repeatedly.
   If the first repeated value is x itself, x belongs to a cycle
   and is therefore not part of the sequence.
   All other integers are transient and are listed in increasing order.
   a(n) is the nth transient integer (offset 1). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static bool is_transient(seq_t start)
{
    /* follow the orbit of start until a value repeats */
    seq_t cur = start;
    seq_t visited[128];
    int size = 0;

    while (true) {
        /* linear search for a repeat */
        int i;
        for (i = 0; i < size; ++i) {
            if (visited[i] == cur)
                break;
        }
        if (i < size) {          /* value repeated */
            if (cur == start)    /* cycle includes start */
                return false;    /* not transient */
            else                 /* cycle does not include start */
                return true;     /* transient */
        }

        /* store current value and proceed */
        if (size == 128) {       /* should not happen for this search */
            /* very long orbit – treat as transient to avoid overflow */
            return true;
        }
        visited[size++] = cur;

        /* next value: f(x)=φ(σ(x)) */
        seq_t sigma = sigma_of((idx_t)cur);   /* σ(cur) */
        cur = totient_seq(sigma);                /* φ(σ(cur)) */
    }
}

seq_t a(idx_t n)
{
    /* n ≥ 1 – generate transient numbers in increasing order */
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        if (is_transient((seq_t)m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
