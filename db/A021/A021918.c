/*
 * A021918  Decimal expansion of 1/914.
 * https://oeis.org/A021918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (⌊10^(n+1) / 914⌋ mod 10) – the n‑th digit after the
 *            decimal point of 1/914
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The remainder never exceeds 913, so unsigned long long is safe for intermediate */
    unsigned long long rem = 1;
    seq_t digit = 0;

    /* Perform n+1 long‑division steps to reach the digit at position n */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;                 /* bring down next digit */
        digit = (seq_t)(rem / 914ULL); /* next decimal digit */
        rem   = rem % 914ULL;          /* update remainder */
    }
    return digit;
}
