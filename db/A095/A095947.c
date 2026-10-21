/*
 * A095947  Numbers whose name in English contains only the vowel "e".
 * https://oeis.org/A095947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   bad_vowel_in_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A095947 – numbers whose English name contains only the vowel "e".
    We only need to test numbers 1 … 99 because every larger number contains
    the words "hundred", "thousand", etc., which all contain vowels other
    than 'e'.  The function builds the list once (on first use) and
    returns the nth term (1‑based), or 0 if n exceeds the list size. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/bad_vowel_in_word.h"

/* English names for 0–19 */
static const char *const ones[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

/* English names for the tens (20,30,…,90) – index 0 and 1 unused */
static const char *const tens[] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

/* Test whether number n (1 ≤ n ≤ 99) satisfies the property. */
static bool ok_number(int n)
{
    if (n < 20)
        return !bad_vowel_in_word(ones[n]);

    int t = n / 10;          /* tens digit (2–9) */
    int u = n % 10;          /* units digit */

    if (bad_vowel_in_word(tens[t]))
        return false;        /* tens part has forbidden vowel */

    if (u == 0)
        return true;         /* no units part */

    /* units part must also avoid forbidden vowels */
    return !bad_vowel_in_word(ones[u]);
}

/* Cache of computed sequence terms.  The sequence is finite (12 terms).   */
static seq_t seq[12];
static size_t seq_len = 0;

/* Initialise cache on first call. */
static void init_seq(void)
{
    if (seq_len != 0)          /* already initialised */
        return;

    for (int i = 1; i <= 99; ++i)
        if (ok_number(i))
            seq[seq_len++] = (seq_t)i;   /* store the term */
}

/* Return the nth term of the sequence, 1‑based.  If n exceeds the number
   of known terms the function returns 0 (the sequence ends).              */
seq_t a(idx_t n)
{
    init_seq();
    if (n == 0)          /* behaviour below offset is undefined      */
        return 0;
    if (n <= seq_len)
        return seq[n - 1];
    return 0;            /* n exceeds sequence length                   */
}
