/*
 * A103303  Complete list of digits used in the counting numbers (in base 10).
 *          Also known as the "Arabic numerals".
 * https://oeis.org/A103303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   pow10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_idx.h"
/*  a(n) = the n-th digit in the infinite string
     S = "0123456789101112…",
     obtained by concatenating the decimal representations of all
     non‑negative integers starting with 0.  */

seq_t a(idx_t n)
{
    /* 'remaining' counts how many digits we still need to skip
       (1‑based).  Everything below is in seq_t to avoid overflow. */
    seq_t remaining = (seq_t)n;

    idx_t len   = 1;            /* current digit‑length of the numbers */
    seq_t count = (seq_t)10;    /* how many numbers with this length
                                  0–9 for len = 1, 10–99 for len = 2 … */
    seq_t blockDigits = count * (seq_t)len;  /* total digits in this block */

    /* Skip whole blocks until the desired digit is inside the current block */
    while (remaining > blockDigits) {
        remaining -= blockDigits;
        ++len;
        if (len == 2)          /* numbers 10–99 : 9 numbers */
            count = (seq_t)9;
        else                   /* numbers 100–999… : 9·10^(len-2) numbers */
            count *= (seq_t)10;
        blockDigits = count * (seq_t)len;
    }

    /* Now the desired digit lies inside this block of numbers of length 'len'. */
    idx_t digitIdx     = (idx_t)((remaining - 1) % (seq_t)len);   /* 0‑based index inside the number */
    idx_t numberOffset = (idx_t)((remaining - 1) / (seq_t)len);   /* 0‑based index of the number in this block */

    seq_t firstNum = (len == 1) ? (seq_t)0 : pow10_idx(len - 1);   /* first number with this length */
    seq_t theNumber = firstNum + (seq_t)numberOffset;             /* the actual number containing the digit */

    seq_t divisor = pow10_idx(len - 1 - digitIdx);                 /* 10^(len‑1‑digitIdx) */
    seq_t digit   = (theNumber / divisor) % (seq_t)10;             /* extract the required digit */

    return digit;
}
