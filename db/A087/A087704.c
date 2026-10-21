/*
 * A087704  Number of steps for iteration of map x -> (5/3)*floor(x) to reach
 *          an integer > n when started at n, or -1 if no such integer is ever
 *          reached.
 * https://oeis.org/A087704
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2488, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A087704: number of steps needed to reach an integer > n
   by iterating the map x → (5/3)*floor(x), starting from the integer n.
   The algorithm follows the definition directly, performing at most a
   fixed large number of iterations before giving up. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. */
    if (n < (idx_t)2) return (seq_t)-1;

    seq_t num = n;          /* current value as a rational num/den */
    seq_t den = 1;
    idx_t steps = 0;
    const idx_t MAX_ITERS = 2000000;   /* safety limit */

    for (;;) {
        /* floor(x) is numerator divided by denominator (all values ≥ 0). */
        seq_t fl = num / den;

        /* Compute next numerator = 5 * floor(x). */
        seq_t next_num = 5 * fl;
        seq_t next_den = 3;

        /* Reduce if divisible by 3: write as an integer. */
        if (next_num % 3 == 0) {
            next_num /= 3;
            next_den = 1;
        }

        /* Update the current value. */
        num = next_num;
        den = next_den;
        ++steps;

        /* Check for an integer > the starting value. */
        if (num % den == 0) {
            seq_t val = num / den;
            if (val > n) return (seq_t)steps;
        }

        /* Safety stop to avoid infinite loops. */
        if (steps > MAX_ITERS) break;
    }
    return (seq_t)-1;   /* No integer reachable within the limit. */
}
