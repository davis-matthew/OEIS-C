/*
 * A083514  Number of steps for iteration of map x -> (4/3)*ceiling(x) to
 *          reach an integer > 3n+1 when started at 3n+1, or -1 if no such
 *          integer is ever reached.
 * https://oeis.org/A083514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083514: Number of steps for iteration of map
   x -> (4/3)*ceil(x) to reach an integer > 3n+1
   when started at 3n+1, or -1 if no such integer is reached. */

/* Uses only integer arithmetic; the numerator is kept as
   a rational number with denominator 1 or 3. */

seq_t a(idx_t n) {
    /* Start value: 3n+1 */
    seq_t start = (seq_t)n * 3 + 1;
    seq_t num   = start;   /* numerator of the current value */
    seq_t den   = 1;       /* denominator (1 or 3) */
    seq_t steps = 0;       /* steps taken */

    /* Upper bound on steps to avoid infinite loops if
       an integer is never reached.  In practice the
       sequence is conjectured to always terminate. */
    const idx_t MAX_STEPS = 100000;

    for (idx_t i = 0; i < MAX_STEPS; ++i) {
        /* Check termination condition: integer and > start */
        if (den == 1 && num > start)
            return steps;

        /* ceil(num/den) = (num + den - 1) / den   */
        seq_t ceilv = (num + den - 1) / den;

        /* Multiply by 4 and then divide by 3 */
        unsigned __int128 prod = (unsigned __int128)ceilv * 4u;

        if (prod % 3u == 0) {
            num = (seq_t)(prod / 3u);
            den = 1;
        } else {
            num = (seq_t)prod; /* numerator stays; denominator becomes 3 */
            den = 3;
        }

        ++steps;
    }

    /* If we reach this point, no integer was encountered. */
    return (seq_t)-1;
}
