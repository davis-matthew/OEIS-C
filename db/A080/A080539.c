/*
 * A080539  Number of neutrons in longest known radioactive decay series
 *          ending with Lead 208 ("thorium series"), reversed.
 * https://oeis.org/A080539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2*n + 119 for n >= 16 (after the 16th entry the chain consists solely of
 * alpha decays, each adding 2 neutrons).  The first 16 terms are listed here
 * to satisfy the definition for n < 16. */
seq_t a(idx_t n)
{
    /* First 16 values of the sequence */
    static const seq_t init[16] = {
        126, 127, 129, 130, 132, 134, 136, 137,
        139, 140, 142, 144, 146, 148, 150, 152
    };

    if (n < 16)
        return init[n];

    /* For n >= 16 the sequence increments by 2 each step:
     * a(n) = 2*n + 119 (determined from a(16) = 151). */
    return (seq_t)2 * n + (seq_t)119;
}
