/*
 * A234577  Let S_n = 0 followed by base-2 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8325, u16 n <= 8325, u32 n <= 8325, u64 n <= 8325,
 *            u128 n <= 8325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234577: a(n) = the n-th digit (0‑based) of the infinite string obtained
   by concatenating, for each i ≥ 0, the string S_i = '0' concatenated with
   the binary representation of i written with its bits in reverse order. */

seq_t a(idx_t n)
{
    idx_t pos = n;      /* remaining index within the concatenated string   */
    idx_t i = 0;        /* current value whose block we are examining     */

    while (1) {
        idx_t len;      /* length of block S_i                            */

        if (i == 0) {
            /* For i = 0: binary(0) = "0"; reversed = "0";
               prepend a leading '0' → "00" → length 2.          */
            len = 2;
        } else {
            /* Number of bits in i (no leading zeros).          */
            idx_t bitlen = 64 - __builtin_clzll((unsigned long long)i);
            /* S_i = '0' + reversed binary(i)   → prepend 0 + bitlen. */
            len = bitlen + 1;
        }

        if (pos < len) break;   /* pos lies inside block S_i                */
        pos -= len;             /* skip this block and continue             */
        ++i;
    }

    /* Inside block S_i: the first character is always 0.            */
    if (pos == 0) return (seq_t)0;

    /* pos ≥ 1: this is the (pos‑1)‑th bit of the reversed binary(i),
       i.e. the (pos‑1)‑th least significant bit of i.             */
    return (seq_t)((i >> (pos - 1)) & 1);
}
