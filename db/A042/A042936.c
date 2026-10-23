/*
 * A042936  Numerators of continued fraction convergents to sqrt(1000).
 * https://oeis.org/A042936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042936 – Numerators of continued‑fraction convergents to sqrt(1000)
 *
 * Formula: a(0) = 31
 *          a(n) = a[n] * a(n‑1) + a(n‑2)  for n ≥ 1
 * where a[n] is the n‑th partial quotient of the simple continued
 * fraction of √1000.  The partial quotients are
 *   a0 = 31,  a1 = 1,  a2 = 2,  a3 = 1,  a4 = 31,  a5 = 62,
 *   then the block {1, 2, 1, 31, 62} repeats indefinitely.
 *
 * The implementation builds that block once and evaluates the
 * recurrence for any requested index n.  No memoisation is needed
 * because n ≈ 10 000 is trivial to compute in O(n).
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

static uint64_t *period_vals = NULL; /* block 1,2,1,31,62 */
static size_t   period_len   = 0;     /* 5 */

/* Build the period of the continued fraction for sqrt(1000) once. */
static void init_period(void)
{
    if (period_len > 0)           /* already built */
        return;

    const uint64_t D  = 1000;
    const uint64_t a0 = 31;      /* floor(sqrt(1000)) */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    uint64_t a_next;
    /* the period ends when a == 2*a0 = 62 */
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a_next = (a0 + m) / d;

        /* store the partial quotient a_next (≡ a[k]) */
        period_vals = (uint64_t *)realloc(period_vals, (period_len + 1) * sizeof(uint64_t));
        period_vals[period_len++] = a_next;

        a = a_next;
    } while (a_next != 2 * a0);
}

/* Return the n-th term of the sequence.  n is 0‑based. */
seq_t a(idx_t n)
{
    /* Build period first time */
    if (period_len == 0)
        init_period();

    /* Convergent h[-1] = 1, h[0] = a0 = 31 */
    const seq_t a0 = (seq_t)31;
    seq_t prev2 = (seq_t)1;   /* h[-1] */
    seq_t prev1 = a0;         /* h[0] */

    if (n == 0)
        return prev1;

    /* i = 1 computes h[1] */
    seq_t ai = (seq_t)period_vals[0];            /* a1 = 1 */
    seq_t cur = ai * prev1 + prev2;
    prev2 = prev1;
    prev1 = cur;

    if (n == 1)
        return prev1;

    /* For i ≥ 2 */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        ai = (seq_t)period_vals[(i - 1) % period_len];
        cur = ai * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
