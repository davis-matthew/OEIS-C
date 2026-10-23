/*
 * A174879  Decimal representation of consonants and vowels in English
 *          integers mapped to binary 0 and 1.
 * https://oeis.org/A174879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   a(n) = binary number obtained by writing the English words of n
 *            (without spaces or hyphens) and replacing vowels a,e,i,o,u with
 *            1 and consonants with 0, then interpreting the resulting binary
 *            string as a decimal integer
 * Range:     u8 n <= 22, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Vowel test */
static int is_vowel(char c)
{
    return (c=='a' || c=='e' || c=='i' || c=='o' || c=='u');
}

/* push the binary bits of an English word onto *value */
static void push_word(seq_t *value, const char *s)
{
    while (*s)
    {
        *value = ((*value << 1) | (seq_t)is_vowel(*s));
        ++s;
    }
}

/* English words for numbers.  All words are lower‑case, no spaces. */
static const char *const ones[10] = {
    "zero","one","two","three","four","five","six","seven","eight","nine"
};
static const char *const teens[10] = {
    "ten","eleven","twelve","thirteen","fourteen","fifteen",
    "sixteen","seventeen","eighteen","nineteen"
};
static const char *const tens[10] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

/* Recursively build the bit representation for n. */
static void build_number(idx_t n, seq_t *value)
{
    if (n >= 1000000)   /* millions – not needed for n≤10000, handled for safety */
    {
        idx_t millions = n / 1000000;
        build_number(millions, value);
        push_word(value, "million");
        n %= 1000000;
        if (!n) return;
    }
    if (n >= 1000)
    {
        idx_t thousands = n / 1000;
        build_number(thousands, value);
        push_word(value, "thousand");
        n %= 1000;
        if (!n) return;
    }
    if (n >= 100)
    {
        idx_t hundreds = n / 100;
        build_number(hundreds, value);
        push_word(value, "hundred");
        n %= 100;
        if (!n) return;
    }
    if (n >= 20)
    {
        idx_t t = n / 10;
        push_word(value, tens[t]);
        n %= 10;
        if (!n) return;
    }
    if (n >= 10)
    {
        push_word(value, teens[n - 10]);
        return;
    }
    /* 1–9 */
    push_word(value, ones[(int)n]);
}


/* External linkage as required by the build. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    build_number(n, &result);
    return result;
}
