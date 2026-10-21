/*
 * A081361  Triangle read by rows: T(n,1) = number of letters in English name
 *          for n, T(n,k+1) = number of letters in T(n,k), but stop if
 *          T(n,k+1) would equal T(n,k).
 * https://oeis.org/A081361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn, tabf, less
 * Formula:   a(n) is the triangle read by rows defined as follows: T(n,1) =
 *            number of letters in the English name for n (without hyphens or
 *            spaces). For k ≥ 1, T(n,k+1) = number of letters in T(n,k) but
 *            stop when the next value would equal the current value. a(n) is
 *            the sequence obtained by listing all T(n,k)...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper tables for letter counts. */
static const unsigned char len0to19[20] = {
    4,  /* 0 -> "zero"   */
    3,  /* 1 -> "one"    */
    3,  /* 2 -> "two"    */
    5,  /* 3 -> "three"  */
    4,  /* 4 -> "four"   */
    4,  /* 5 -> "five"   */
    3,  /* 6 -> "six"    */
    5,  /* 7 -> "seven"  */
    5,  /* 8 -> "eight"  */
    4,  /* 9 -> "nine"   */
    3,  /*10 -> "ten"    */
    6,  /*11 -> "eleven" */
    6,  /*12 -> "twelve" */
    8,  /*13 -> "thirteen"*/
    8,  /*14 -> "fourteen"*/
    7,  /*15 -> "fifteen"*/
    7,  /*16 -> "sixteen"*/
    9,  /*17 -> "seventeen"*/
    8,  /*18 -> "eighteen"*/
    8   /*19 -> "nineteen"*/
};
static const unsigned char tens_len[10] = {
    0, 0, 6, 6, 5, 5, 5, 7, 6, 6 /* 20 "twenty", 30 "thirty", 40 "forty", ... */
};
static const unsigned char power_len[10] = {
    0,          /* 10^0   */
    8,          /* thousand */
    7,          /* million  */
    8,          /* billion  */
    9,          /* trillion */
    8,          /* quadrillion */
    10,         /* quintillion */
    11,         /* sextillion  */
    12,         /* septillion */
    14          /* octillion   */
};

/* Count letters in a number less than 20. */
static unsigned long long count_two_digits(unsigned long long d)
{
    if (d < 20) return len0to19[(unsigned int)d];
    unsigned int tens = (unsigned int)(d / 10);
    unsigned int units = (unsigned int)(d % 10);
    return tens_len[tens] + (units ? len0to19[units] : 0);
}

/* Count letters in a number less than 1000. */
static unsigned long long count_three_digits(unsigned long long d)
{
    if (d < 100) return count_two_digits(d);
    unsigned int hundreds = (unsigned int)(d / 100);
    unsigned long long rem = d % 100;
    unsigned long long res = len0to19[hundreds] + 7; /* "hundred" */
    if (rem) res += count_two_digits(rem);
    return res;
}

/* Count letters in any unsigned 64‑bit integer. */
static unsigned long long count_letters_val(unsigned long long num)
{
    if (num == 0) return 4;          /* "zero" */
    unsigned long long res = 0;
    unsigned int level = 0;
    while (num) {
        unsigned long long grp = num % 1000;
        if (grp) {
            res += count_three_digits(grp);
            if (level > 0) res += power_len[level];
        }
        num /= 1000;
        level++;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t row   = 1;
    while (1) {
        unsigned long long prev = count_letters_val((unsigned long long)row);
        while (1) {
            count++;
            if (count == n) return (seq_t)prev;
            unsigned long long next = count_letters_val(prev);
            if (next == prev) break;
            prev = next;
        }
        row++;
    }
}
