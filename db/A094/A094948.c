/*
 * A094948  Number of letters in n^2.
 * https://oeis.org/A094948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = A005589(A000290(n)) – number of English letters in n²
 * Range:     u8 n <= 15, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Support tables for number names */
static const int unit_len[20] = {
    4, /* zero  */
    3, /* one   */
    3, /* two   */
    5, /* three */
    4, /* four  */
    4, /* five  */
    3, /* six   */
    5, /* seven */
    5, /* eight */
    4, /* nine  */
    3, /* ten   */
    6, /* eleven*/
    6, /* twelve*/
    8, /* thirteen*/
    8, /* fourteen*/
    7, /* fifteen*/
    7, /* sixteen*/
    9, /* seventeen*/
    8, /* eighteen*/
    8  /* nineteen*/
};

static const int tens_len[10] = {
    0, /* 0 */
    0, /* 1 */
    6, /* twenty */
    6, /* thirty */
    5, /* forty  */
    5, /* fifty  */
    5, /* sixty  */
    7, /* seventy*/
    6, /* eighty */
    6  /* ninety */
};

static const int scale_len[13] = {
    0,  /* units      */
    8,  /* thousand   */
    7,  /* million    */
    7,  /* billion    */
    8,  /* trillion   */
    11, /* quadrillion*/
    11, /* quintillion*/
    10, /* sextillion */
    10, /* septillion */
    9,  /* octillion  */
    9,  /* nonillion  */
    9,  /* decillion  */
    9   /* undecillion – not needed for typical ranges */
};

/* Count letters in a three‑digit group (1–999). 0 returns 0. */
static seq_t count_3digit(seq_t n)
{
    seq_t letters = 0;
    if (n == 0) return 0;

    seq_t hundreds = n / 100;
    seq_t rem      = n % 100;

    if (hundreds) {
        letters += (seq_t)unit_len[(int)hundreds];
        letters += 7;   /* "hundred" */
    }

    if (rem >= 20) {
        seq_t tens  = rem / 10;
        seq_t ones  = rem % 10;
        letters += (seq_t)tens_len[(int)tens];
        if (ones) letters += (seq_t)unit_len[(int)ones];
    } else if (rem) {
        letters += (seq_t)unit_len[(int)rem];
    }

    return letters;
}

/* Count letters in the English word for n (n ≥ 0). */
static seq_t count_letters(seq_t n)
{
    if (n == 0) return (seq_t)unit_len[0];   /* "zero" */

    seq_t total = 0;
    int   scale = 0;   /* units type */

    while (n > 0) {
        seq_t group = n % 1000;
        if (group) {
            total += count_3digit(group);
            if (scale > 0) total += (seq_t)scale_len[scale];
        }
        n /= 1000;
        ++scale;
    }
    return total;
}

/* OEIS A094948: number of letters in n². */
seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    return count_letters(sq);
}
