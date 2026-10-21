/*
 * A075774  Number of syllables in n in American English.
 * https://oeis.org/A075774
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

/* Computes OEIS A075774: number of syllables in the American English
 * verbal representation of the non‑negative integer n.
 *
 * The algorithm decomposes n into groups of three digits (units,
 * thousands, millions, …).  For each group it counts the syllables of
 * the number spelled out (hundreds, tens, units, teens) and adds the
 * syllable count of the scale word (thousand, million, etc.) if the
 * group is non‑zero.  All arithmetic is integer; the routine works
 * for any n that fits in idx_t and returns the result in seq_t.
 */

static const unsigned char units_syll[10] = {
    2, /* zero */
    1, /* one  */
    1, /* two  */
    1, /* three*/
    1, /* four */
    1, /* five */
    1, /* six  */
    2, /* seven */
    1, /* eight */
    1  /* nine  */
};

static const unsigned char teens_syll[10] = {
    1, /* ten   */
    3, /* eleven*/
    1, /* twelve*/
    2, /* thirteen*/
    2, /* fourteen*/
    2, /* fifteen */
    2, /* sixteen */
    3, /* seventeen*/
    2, /* eighteen */
    2  /* nineteen */
};

static const unsigned char tens_syll[10] = {
    0, /* 0 unused */
    0, /* 1 unused (teens handled above) */
    2, /* twenty   */
    2, /* thirty   */
    2, /* forty    */
    2, /* fifty    */
    2, /* sixty    */
    3, /* seventy  */
    2, /* eighty   */
    2  /* ninety   */
};

static const unsigned char scale_syll[5] = {
    0, /* units (no scale word) */
    2, /* thousand */
    3, /* million  */
    3, /* billion  */
    3  /* trillion */
};

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)units_syll[0];   /* "zero" */
    }

    seq_t total = 0;
    idx_t   group_idx = 0;   /* 0: units, 1: thousands, 2: millions, … */
    idx_t   temp = n;

    while (temp > 0) {
        idx_t g = temp % 1000;    /* current 3‑digit group */
        temp  /= 1000;

        if (g != 0) {
            unsigned int gcnt = 0;

            /* hundreds part */
            idx_t h = g / 100;
            idx_t rest = g % 100;
            if (h != 0) {
                gcnt += units_syll[h];
                gcnt += 2;        /* "hundred" has two syllables */
            }

            /* tens / units / teens part */
            if (rest != 0) {
                if (rest < 10) {
                    gcnt += units_syll[rest];
                } else if (rest < 20) {
                    gcnt += teens_syll[rest - 10];
                } else {
                    idx_t tdigit = rest / 10;
                    idx_t udigit = rest % 10;
                    gcnt += tens_syll[tdigit];
                    if (udigit != 0) {
                        gcnt += units_syll[udigit];
                    }
                }
            }

            /* scale word for this group (if not units) */
            if (group_idx > 0 && group_idx < 5) {
                gcnt += scale_syll[group_idx];
            }

            total += (seq_t)gcnt;
        }

        ++group_idx;
    }

    return total;
}
