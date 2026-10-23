/*
 * A117817  Let T_n be the infinite sequence formed by starting with 1 and
 *          repeatedly reversing the digits and adding n to get the next term.
 *          If T_n eventually reaches a cycle, sequence gives length of that
 *          cycle, otherwise -1.
 * https://oeis.org/A117817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = length of the cycle (in the reverse‑and‑add sequence *
 *            starting with 1 and repeatedly adding n) or -1 if the sequence
 *            never * enters a cycle (i.e., diverges or reaches a known
 *            non‑terminating pattern)
 * Range:     i8 n <= 4, i16 n <= 114, i32 n <= 114, i64 n <= 114,
 *            i128 n <= 114, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Reverse the decimal digits of an unsigned 64‑bit integer. */
static idx_t rev_uint64(idx_t x)
{
    idx_t rev = 0;
    while (x != 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Next term of the reverse‑and‑add sequence for a given addend. */
static idx_t next_term(idx_t cur, idx_t add)
{
    return rev_uint64(cur) + add;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Early rejection: multiples of 10 never enter a cycle. */
    if ((n % 10) == 0)
        return (seq_t)-1;

    /* Floyd’s cycle‑finding algorithm. */
    const size_t MAX_STEPS = 1000000;   /* safe upper bound for practical n */
    size_t steps = 0;

    idx_t tortoise = next_term((idx_t)1, n);
    idx_t hare = next_term(next_term((idx_t)1, n), n);

    while (tortoise != hare) {
        if (++steps > MAX_STEPS)
            return (seq_t)-1;          /* diverging or too long to analyze */

        tortoise = next_term(tortoise, n);
        hare = next_term(next_term(hare, n), n);
    }

    /* Find start of the cycle (mu). */
    idx_t tortoise_start = (idx_t)1;
    while (tortoise_start != tortoise) {
        tortoise_start = next_term(tortoise_start, n);
        tortoise = next_term(tortoise, n);
    }

    /* Find cycle length (lambda). */
    size_t lambda = 1;
    idx_t hare2 = next_term(tortoise_start, n);
    while (tortoise_start != hare2) {
        hare2 = next_term(hare2, n);
        ++lambda;
    }

    return (seq_t)lambda;
}
