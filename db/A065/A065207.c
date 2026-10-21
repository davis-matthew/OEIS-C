/*
 * A065207  Numbers which need two 'Reverse and Add' steps to reach a
 *          palindrome.
 * https://oeis.org/A065207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A065207 – numbers that require exactly two reverse‑and‑add steps
 *            to reach a palindrome
 * Range:     u8 n <= 3, u16 n <= 1474, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes terms on demand, without storing an array, and stops if the
   counting variable overflows (happens only if the sequence value would exceed the
   range of seq_t).   */
#include <stdbool.h>

/* Reverse the decimal representation of v.  Works for all seq_t types. */
static seq_t rev_digits(seq_t v)
{
    seq_t rev = (seq_t)0;
    while (v > (seq_t)0) {
        rev = rev * (seq_t)10 + v % (seq_t)10;
        v   = v / (seq_t)10;
    }
    return rev;
}

/* Test whether v is a palindrome. */
static bool is_pal(seq_t v)
{
    return v == rev_digits(v);
}

/* Returns true iff v needs exactly two reverse‑and‑add steps to become a palindrome.  */
static bool requires_two_steps(seq_t v)
{
    if (is_pal(v))
        return false;                               /* 0 steps – not counted */

    seq_t step1 = v + rev_digits(v);
    if (is_pal(step1))
        return false;                               /* 1 step – not counted */

    seq_t step2 = step1 + rev_digits(step1);
    return is_pal(step2);                           /* true only for 2 steps */
}

/* The sequence A065207.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for n <= 0 */

    idx_t count = 0;
    seq_t cand  = (seq_t)1;
    seq_t last  = (seq_t)0;

    while (count < n) {
        if (requires_two_steps(cand)) {
            ++count;
            last = cand;
            if (count == n)
                return last;
        }

        /* Detect overflow of cand; if it wraps to zero we can no longer find more terms. */
        seq_t next = cand + (seq_t)1;
        if (next <= cand)
            break;  /* overflow */
        cand = next;
    }

    /* If we reach here, we could not find n terms within the range of seq_t. */
    return (seq_t)0;
}
