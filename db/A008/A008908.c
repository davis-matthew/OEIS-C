/*
 * A008908  a(n) = (1 + number of halving and tripling steps to reach 1 in the
 *          Collatz (3x+1) problem), or -1 if 1 is never reached.
 * https://oeis.org/A008908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look, changed
 * Formula:   a(n) = A006577(n) + 1 * A006577(n) = number of Collatz steps
 *            needed to reach 1 (using the map * n→3n+1 for odd n, n→n/2 for
 *            even n). a(n) is that step count plus * one, or -1 if 1 is never
 *            reached
 * Range:     u8 n <= 26, u16 n <= 7014, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A008908.
 * Assumes n ≥ 1 (offset 1).  Any n that does not converge within a
 * practical limit is reported as (seq_t)-1. */
seq_t a(idx_t n)
{
    /* Use seq_t for the Collatz value, idx_t for counting steps. */
    if (n < 1)   /* Undefined below offset; return 0 as a safe fallback. */
        return 0;

    seq_t cur = (seq_t)n;          /* Current Collatz value. */
    idx_t steps = 0;               /* Steps taken. */

    /* Practical safety limit to avoid infinite loops for unproven cases. */
    const idx_t MAX_STEPS = 2000000;

    while (cur != (seq_t)1) {
        if ((cur & (seq_t)1) == (seq_t)0) {
            cur >>= 1;                 /* Even: n = n/2. */
        } else {
            cur = cur * (seq_t)3 + (seq_t)1;   /* Odd: n = 3n+1. */
        }
        ++steps;
        if (steps > MAX_STEPS) {
            /* Convergence not observed within the practical limit. */
            return (seq_t)-1;           /* Represents -1 in unsigned form. */
        }
    }
    /* Return steps + 1 as per the definition. */
    return (seq_t)(steps + 1);
}
