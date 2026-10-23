/*
 * A192776  Squares of binary palindromes.
 * https://oeis.org/A192776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192776: Squares of binary palindromes
 *
 * For n ≥ 1 let b_n be the nth integer whose binary representation is a
 * palindrome (ordered increasingly, starting with 0).  
 * a(n) = decimal interpretation of the binary representation of (b_n)^2.
 * Example: b_4 = 5 (binary 101), b_4^2 = 25 (decimal 25) → binary 11001,
 *          so a(4) = 11001 (decimal 11001).
 */

#include <stdint.h>

/* Reverse the lowest `w` bits of `x`. */
static seq_t reverse_bits(seq_t x, unsigned int w)
{
    seq_t r = 0;
    while (w-- > 0) {
        r = (r << 1) | (x & 1);
        x >>= 1;
    }
    return r;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 1u) return 0;
    if (n == 2u) return 1;

    /* Index among palindromes of length ≥ 2 (0‑based). */
    idx_t idx = n - 3u;
    int len = 2;          /* start with 2‑bit palindromes */

    while (1) {
        int half = (len + 1) / 2;
        idx_t cnt = 1ULL << (half - 1);      /* number of palindromes of this length */
        if (idx < cnt)
            break;                           /* this length contains the desired palindrome */
        idx -= cnt;
        ++len;
    }

    int half = (len + 1) / 2;
    seq_t base = ((seq_t)1ULL << (half - 1)) | (seq_t)idx;  /* left half with leading 1 */

    seq_t palindrome;
    if (len % 2 == 0) {                 /* even length: mirror all bits */
        int k = len / 2;
        seq_t rev = reverse_bits(base, k);
        palindrome = (base << k) | rev;
    } else {                             /* odd length: drop middle bit before mirroring */
        int k = half;                    /* k = ceil(len/2) */
        seq_t rev = reverse_bits(base >> 1, k - 1);
        palindrome = (base << (k - 1)) | rev;
    }

    seq_t square = palindrome * palindrome;

    /* Convert binary representation of `square` into decimal digits. */
    if (square == 0)
        return 0;

    seq_t out = 0;
    /* Count binary digits of square. */
    seq_t tmp = square;
    int bits = 0;
    while (tmp != 0) {
        tmp >>= 1;
        ++bits;
    }

    for (int i = bits - 1; i >= 0; --i) {
        seq_t bit = (square >> i) & 1;
        out = out * 10 + bit;
    }
    return out;
}
