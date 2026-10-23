/*
 * A043274  Sum of the digits of the n-th base 15 palindrome.
 * https://oeis.org/A043274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of the n-th base‑15 palindrome
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t POW15[20] = {
    1ULL,                          /* 15^0 */
    15ULL,                         /* 15^1 */
    225ULL,                        /* 15^2 */
    3375ULL,                       /* 15^3 */
    50625ULL,                      /* 15^4 */
    759375ULL,                     /* 15^5 */
    11390625ULL,                   /* 15^6 */
    170859375ULL,                  /* 15^7 */
    2562890625ULL,                 /* 15^8 */
    38443359375ULL,                /* 15^9 */
    576650390625ULL,               /* 15^10 */
    8649755859375ULL,              /* 15^11 */
    129746337890625ULL,            /* 15^12 */
    1946195068359375ULL,           /* 15^13 */
    29192926025390625ULL,          /* 15^14 */
    437893890380859375ULL,         /* 15^15 */
    656840342041015625ULL,         /* 15^16 */
    9852605130615234375ULL,        /* 15^17 */
    147789077259765625ULL,         /* 15^18 */
    2216836168896484375ULL         /* 15^19 */
};

seq_t a(idx_t n)
{
    /* Handle the first 15 terms – the one‑digit palindromes 0…14. */
    if (n <= 15ULL)
        return (seq_t)(n - 1ULL);   /* digit itself is the sum */

    /* Find the length L of the required palindrome. */
    idx_t rem = n;      /* 1‑based index within the entire sequence */
    idx_t L = 1;        /* current palindrome length being examined */

    while (1) {
        int halfLen = (L + 1) / 2;
        idx_t count;

        if (L == 1) {
            count = 15ULL;
        } else {
            /* For L >= 2: first digit (1–14) times all possibilities for the rest half */
            count = 14ULL * POW15[halfLen - 1];
        }

        if (rem <= count)
            break;      /* the desired palindrome lies in this block */

        rem -= count;
        ++L;
    }

    /* If we ended up with a one‑digit palindrome (should not happen after the first
       15 terms), just return the digit. */
    if (L == 1)
        return (seq_t)(rem - 1ULL);

    /* Determine the digits of the prefix (left half of the palindrome). */
    int halfLen = (L + 1) / 2;          /* length of the prefix */
    idx_t powHalf = POW15[halfLen - 1]; /* 15^(halfLen-1) */

    idx_t j = rem - 1ULL;               /* 0‑based index within this block */

    /* First digit (leading digit, cannot be zero). */
    idx_t firstPart = j / powHalf;          /* 0 … 13 */
    seq_t d0 = (seq_t)firstPart + 1ULL;     /* 1 … 14 */

    /* Sum of all digits in the prefix. */
    seq_t sumDigitsPrefix = d0;

    /* Remaining digits of the prefix, if any. */
    idx_t rest = j % powHalf;

    for (int i = 1; i < halfLen; ++i) {
        idx_t mult = POW15[halfLen - 1 - i];
        seq_t di = (seq_t)((rest / mult) % 15ULL);
        sumDigitsPrefix += di;
    }

    /* Middle digit for odd length palindromes (it is the last digit of the prefix). */
    seq_t midDigit = 0;
    if ((L & 1) && halfLen > 1)      /* L odd and halfLen > 1 */
        midDigit = (seq_t)(rest % 15ULL); /* last digit of the prefix */

    /* Total digit sum of the full palindrome. */
    seq_t result;
    if ((L & 1) == 0)          /* even length */
        result = sumDigitsPrefix * 2;
    else                       /* odd length */
        result = sumDigitsPrefix * 2 - midDigit;

    return result;
}
