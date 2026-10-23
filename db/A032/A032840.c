/*
 * A032840  Numbers whose set of base-16 digits is {3,4}.
 * https://oeis.org/A032840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032840: Numbers whose base‑16 digits are only 3 or 4.
   The nth term (n ≥ 1) is produced by:
      * determining the length L of the hex representation,
      * taking the binary representation of the offset within that length,
      * mapping bits 0→3 and 1→4,
      * evaluating the resulting hex number.   */

seq_t a(idx_t n)
{
    /* --- determine the length L of the number --- */
    idx_t len  = 1;               /* current length of hex digits   */
    idx_t pow2 = 4;               /* 2^(len+1)                       */
    while (pow2 - 2 < n) {
        ++len;
        pow2 <<= 1;
    }

    /* --- offset of n within its length group (1‑based) --- */
    idx_t prevSum = (pow2 >> 1) - 2;     /* sum of counts for lengths < len */
    idx_t offset  = n - prevSum;         /* 1‑based offset in this length   */
    idx_t bits    = offset - 1;          /* 0‑based binary pattern          */

    /* --- build the hex number from bits --- */
    seq_t val = 0;
    idx_t i;
    for (i = 0; i < len; ++i) {
        idx_t bit   = (bits >> (len - 1 - i)) & (idx_t)1;
        seq_t digit = bit ? 4 : 3;
        val = val * 16 + digit;
    }
    return val;
}
