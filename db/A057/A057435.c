/*
 * A057435  Number of letters in the Turkish name of n, excluding spaces and
 *          hyphens.
 * https://oeis.org/A057435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters in the Turkish name of n, ignoring
 *            spaces and hyphens
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const int ones_len[10] = {
    0, /* 0 not used except for n==0 */
    3, /* bir   */
    3, /* iki   */
    2, /* üç    */
    4, /* dört  */
    3, /* beş   */
    4, /* altı  */
    4, /* yedi  */
    5, /* sekiz */
    5  /* dokuz */
};

static const int tens_len[10] = {
    0, /* 0 */
    2, /* on    */
    5, /* yirmi */
    4, /* otuz  */
    4, /* kırk  */
    4, /* elli  */
    6, /* altmış*/
    6, /* yetmiş*/
    6, /* seksen */
    6  /* doksan */
};

static const int mag_len[10] = {
    0,  /* dummy for index 0 */
    3,  /* bin       */
    6,  /* milyon    */
    6,  /* milyar    */
    7,  /* trilyon   */
    9,  /* katrilyon */
    9,  /* kentilyon */
    9,  /* septilyon */
    8,  /* oktylion  */
    8   /* nonilyon  */
};

/* Count the letters of the Turkish representation of a number in [1,999] */
static seq_t count_under_thousand(uint64_t value)
{
    seq_t len = 0;
    uint64_t h = value / 100;
    uint64_t rem = value % 100;
    if (h > 0) {
        if (h == 1)
            len += 3;               /* yuz */
        else
            len += ones_len[h] + 3; /* digit + yuz */
    }
    uint64_t t = rem / 10;
    uint64_t o = rem % 10;
    if (t > 0)
        len += tens_len[t];
    if (o > 0)
        len += ones_len[o];
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 5;              /* sıfır */

    seq_t total = 0;
    uint64_t x = n;
    int group = 0;          /* 0 -> units, 1 -> thousands, 2 -> millions, ... */

    while (x > 0) {
        uint64_t val = x % 1000;
        if (val != 0) {
            if (group == 0) {                     /* units */
                total += count_under_thousand(val);
            } else {
                if (group == 1 && val == 1) {       /* 1,000 is just "bin" */
                    total += mag_len[1];
                } else {
                    total += count_under_thousand(val)
                           + mag_len[group];
                }
            }
        }
        x /= 1000;
        group++;
    }
    return total;
}
