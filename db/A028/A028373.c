/*
 * A028373  Numbers that have only the straight digits {1, 4, 7}.
 * https://oeis.org/A028373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 282, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Representation: Enumerate all numbers whose decimal digits are in {1,4,7}
   by treating n‑1 as a base‑3 number and mapping 0→1, 1→4, 2→7.
   The sequence is then the numbers produced in this order, for n≥1. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is undefined for n=0 */

    /* Find the length l of the block containing the n‑th element.
       Sum of counts of lengths up to l is S(l) = 3 + 3² + … + 3ˡ. */
    idx_t      l   = 1;
    unsigned __int128 pow  = 3;    /* 3^l       */
    unsigned __int128 sum  = 3;    /* S(l)      */
    while (sum < (unsigned __int128)n) {
        l++;
        pow *= 3;
        sum += pow;
    }

    /* Offset of n within the length‑l block (0‑based). */
    unsigned __int128 prev_sum = sum - pow;            /* S(l‑1)   */
    idx_t offset = n - (idx_t)prev_sum - 1;

    /* Extract digits of offset in base‑3 (least significant first). */
    int dig[128];                /* l will never exceed 128 for 64‑bit n */
    idx_t rem = offset;
    for (idx_t i = 0; i < l; ++i) {
        dig[i] = rem % 3;
        rem   /= 3;
    }

    /* Build the decimal number from most significant digit to least. */
    seq_t val = 0;
    for (idx_t i = 0; i < l; ++i) {
        int d = dig[l - 1 - i];
        val = val * 10 + (d == 0 ? 1 : d == 1 ? 4 : 7);
    }

    return val;
}
