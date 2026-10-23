/*
 * A021926  Decimal expansion of 1/922.
 * https://oeis.org/A021926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021926: decimal digits of 1/922
   a(n) = the nth digit after the decimal point in 1/922
   (n = 0 gives the first digit: 0, n = 1 gives second digit: 0, …) */

seq_t a(idx_t n)
{
    /* Remainder after the integer part - starts at 1 for 1/922 */
    uint64_t rem = 1;
    /* The digit at the current position; 0‑9 for all seq_t types */
    seq_t digit = 0;

    /* Produce digits up to and including position n */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;            /* bring down next decimal place */
        digit = (seq_t)(rem / 922ULL); /* current digit (0‑9) */
        rem   %= 922ULL;         /* update remainder for next iteration */
    }

    return digit;
}
