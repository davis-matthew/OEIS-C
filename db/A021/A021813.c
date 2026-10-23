/*
 * A021813  Decimal expansion of 1/809.
 * https://oeis.org/A021813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021813: Decimal digits of 1/809.
   a(n) is the n‑th digit after the decimal point in the decimal
   expansion of 1/809.  Because 809 is prime and coprime to 10,
   the expansion is purely repeating with a cycle length equal
   to the order of 10 modulo 809 (≤ 808). */

#include <stdbool.h>

static bool ready = false;          /* Flag whether digits are precomputed. */
static seq_t digits[809];           /* Storage for the repeating cycle. */
static idx_t period_len = 0;        /* Length of the cycle. */

/* Precompute the entire repeating cycle once.  The cycle length is at
   most 808, so a fixed array is sufficient. */
static void init_cycle(void)
{
    if (ready) return;

    seq_t r = 1;          /* Initial remainder (1 / 809). */
    idx_t i = 0;

    /* Long-division simulation: produce one digit per iteration until
       the remainder repeats (back to 1).  The first digit corresponds
       to a(0). */
    do
    {
        seq_t t    = r * (seq_t)10;
        digits[i]  = t / (seq_t)809;   /* Current digit. */
        r          = t % (seq_t)809;   /* New remainder. */
        ++i;
    } while (r != (seq_t)1);

    period_len = i;    /* Number of digits in the cycle. */
    ready = true;
}

/* Return the n‑th digit of 1/809 (offset 0). */
seq_t a(idx_t n)
{
    init_cycle();                     /* Ensure digits are ready. */
    return digits[n % period_len];    /* Periodic repetition. */
}
