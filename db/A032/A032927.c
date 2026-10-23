/*
 * A032927  Numbers whose set of base-6 digits is {1,2}.
 * https://oeis.org/A032927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base‑6 digits are only 1 or 2 (in
 *            increasing order)
 * Range:     u8 n <= 14, u16 n <= 190, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑bounds, not used by OEIS offset 1 */

    /* Find the length k of the base‑6 representation:
       the first 2^k numbers have exactly k digits (digits are 1 or 2). */
    unsigned int k = 1;
    unsigned __int128 cum = 0;      /* cumulative count of terms up to current k */
    unsigned __int128 pow2 = 2;     /* 2^k  (starts with 2^1) */

    while (1) {
        cum += pow2;                /* add count for current length k */
        if ((unsigned __int128)n <= cum) break;
        pow2 <<= 1;                 /* prepare for next length k+1 */
        ++k;
    }

    /* t is the zero‑based index within the k‑digit block   */
    unsigned __int128 prev = cum - pow2;          /* counts before current block */
    unsigned __int128 t = (unsigned __int128)n - prev - 1;

    seq_t result = 0;
    /* Convert binary number t into a base‑6 number using digits 1 and 2.
       Process bits from most significant (k‑1) to least (0). */
    for (int pos = (int)k - 1; pos >= 0; --pos) {
        unsigned int bit = (unsigned int)((t >> pos) & 1);
        seq_t digit = 1 + (seq_t)bit;   /* 0 -> 1, 1 -> 2 */
        result = result * 6 + digit;
    }

    return result;
}
