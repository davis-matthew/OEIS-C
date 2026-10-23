/*
 * A161980  a(n) = the smallest positive multiple of n containing {the string
 *          made by reversing the order of the binary digits of n, and
 *          removing leading 0's} as a substring when a(n) is written in
 *          binary.
 * https://oeis.org/A161980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A161980: a(n) = the smallest positive multiple of n whose binary
   representation contains the reverse of the binary digits of n
   (with leading zeros removed) as a substring. */

#include <stdint.h>
#include <stdbool.h>

/* bit length for idx_t (uint64_t) */
static int bitlen_u64(idx_t x)
{
    int len = 0;
    while (x) { x >>= 1; ++len; }
    return len;
}

/* bit length for seq_t (generic unsigned integer or big‑integer type) */
static int bitlen_seq(seq_t x)
{
    int len = 0;
    while (x) { x >>= 1; ++len; }
    return len;
}

/* Reverse the binary digits of n (no leading zeros). */
static seq_t reverse_bits(idx_t n)
{
    int len = bitlen_u64(n);
    seq_t rev = 0;
    for (int i = 0; i < len; ++i)
        if (n & (((idx_t)1) << i))
            rev |= ((seq_t)1 << (len - 1 - i));
    return rev;
}

/* Return true if pattern of length patlen bits appears in value. */
static bool contains_substring(seq_t value, seq_t pattern, int patlen)
{
    int val_len = bitlen_seq(value);
    if (patlen > val_len) return false;
    seq_t mask = (((seq_t)1 << patlen) - 1);
    for (int pos = 0; pos <= val_len - patlen; ++pos)
        if (((value >> pos) & mask) == pattern)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n = 0, safe guard */

    seq_t rev = reverse_bits(n);
    int patlen = bitlen_seq(rev);

    seq_t k = 1;
    while (true)
    {
        seq_t m = ((seq_t)n) * ((seq_t)k);
        if (contains_substring(m, rev, patlen))
            return m;
        ++k;
    }
}
