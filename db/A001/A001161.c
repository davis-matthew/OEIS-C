/*
 * A001161  Numbers containing an even number of letters.
 * https://oeis.org/A001161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   search -- a(n) is the n-th k >= 0 whose English name (no "and",
 *            American short scale) has an even number of letters
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int english_letter_count(seq_t num) {
    static const int ones_letters[] = {4, 3, 3, 5, 4, 4, 3, 5, 5, 4};
    static const int teens_letters[] = {3, 6, 6, 8, 8, 7, 7, 9, 8, 8};
    static const int tens_letters[] = {0, 0, 6, 6, 5, 5, 5, 7, 6, 6};
    /* thousand, million, billion, trillion, quadrillion, quintillion,
       sextillion, septillion, octillion, nonillion, decillion,
       undecillion, duodecillion */
    static const int scale_words_letters[] = {0, 8, 7, 7, 8, 11, 11, 10, 10, 9, 9, 9, 11, 12};

    if (num == 0) return ones_letters[0];

    int total = 0;
    int scale = 0;

    while (num > 0 && scale < 14) {
        seq_t group = num % (seq_t)1000;

        if (group > 0) {
            seq_t h = group / (seq_t)100;
            if (h > 0) {
                total += ones_letters[(int)(long long)h];
                total += 7; /* hundred */
            }

            seq_t remainder = group % (seq_t)100;
            if (remainder >= 10 && remainder < 20) {
                total += teens_letters[(int)(long long)(remainder - 10)];
            } else {
                seq_t t = remainder / (seq_t)10;
                seq_t o = remainder % (seq_t)10;
                if (t > 0) total += tens_letters[(int)(long long)t];
                if (o > 0) total += ones_letters[(int)(long long)o];
            }

            if (scale > 0) total += scale_words_letters[scale];
        }

        num /= (seq_t)1000;
        scale++;
    }

    return total;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 0;

    while (1) {
        if (english_letter_count(num) % 2 == 0) {
            count++;
            if (count == n) return num;
        }
        num++;
    }
}
