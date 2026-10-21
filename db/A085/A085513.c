/*
 * A085513  Number of "e"s in n (in English).
 * https://oeis.org/A085513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count of 'e's in the English word for each unit, teen, and tens. */
/* 0‑9 */
static const seq_t unit_counts[10] = {
    1, /* zero   */
    1, /* one    */
    0, /* two    */
    2, /* three  */
    0, /* four   */
    1, /* five   */
    0, /* six    */
    2, /* seven  */
    1, /* eight  */
    1  /* nine   */
};

/* 10‑19 */
static const seq_t teen_counts[10] = {
    1, /* ten     */
    3, /* eleven  */
    2, /* twelve  */
    2, /* thirteen */
    2, /* fourteen */
    2, /* fifteen  */
    2, /* sixteen  */
    4, /* seventeen */
    3, /* eighteen */
    3  /* nineteen  */
};

/* 20‑90 */
static const seq_t tens_counts[10] = {
    0, /* unused    */
    0, /* unused    */
    1, /* twenty    */
    0, /* thirty    */
    0, /* forty     */
    0, /* fifty     */
    0, /* sixty     */
    2, /* seventy   */
    1, /* eighty    */
    1  /* ninety    */
};

/* Return the number of 'e's in the English words for a number 1 ≤ n < 1000. */
static seq_t count_e_below_1000(idx_t n)
{
    if (n < 10) {
        return unit_counts[n];
    }
    if (n < 20) {
        return teen_counts[n - 10];
    }
    if (n < 100) {
        int tens   = n / 10;
        int units  = n % 10;
        if (units == 0) {
            return tens_counts[tens];
        }
        return tens_counts[tens] + unit_counts[units];
    }

    /* 100 ≤ n < 1000 */
    seq_t hundred_part = unit_counts[n / 100] + 1;   /* “hundred” has 1 e */
    idx_t rem = n % 100;
    if (rem == 0) {
        return hundred_part;
    }
    return hundred_part + count_e_below_1000(rem);
}

/* Main entry: a(n) = number of 'e's in the English representation of n. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return unit_counts[0];  /* “zero” → 1 e */
    }

    seq_t total = 0;
    while (n > 0) {
        idx_t group = n % 1000;      /* process a 3‑digit block */
        if (group > 0) {
            total += count_e_below_1000(group);
        }
        n /= 1000;                   /* move to the next magnitude */
    }
    return total;
}
