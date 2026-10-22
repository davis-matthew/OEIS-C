/*
 * A005606  Position of first letter of n (in English) in alphabet.
 * https://oeis.org/A005606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  word, nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <ctype.h>

static const char *ones[] = {
    "Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
    "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen",
    "Seventeen", "Eighteen", "Nineteen"
};

static const char *tens[] = {
    "", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"
};

seq_t a(idx_t n) {
    /* Position of first letter of English name of n in the alphabet */
    
    // Find the most significant 3-digit group
    idx_t temp = n;
    idx_t group = 0;
    
    while (temp > 0) {
        group = temp % 1000;
        temp /= 1000;
    }
    
    // Determine which word the group starts with
    const char *word;
    
    if (group < 20) {
        word = ones[(long long)group];
    } else if (group < 100) {
        word = tens[(long long)(group / 10)];
    } else {
        word = ones[(long long)(group / 100)];
    }
    
    // Get the first letter and return its position (1-indexed)
    char first = toupper(word[0]);
    return (seq_t)(first - 'A' + 1);
}
