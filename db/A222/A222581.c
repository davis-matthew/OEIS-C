/*
 * A222581  Run lengths of digits when concatenating Roman numerals less than
 *          4000, cf. A093796.
 * https://oeis.org/A222581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9887, u16 n <= 9887, u32 n <= 9887, u64 n <= 9887,
 *            u128 n <= 9887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the run lengths of digits in the concatenation of Roman
   numerals for numbers 1 … 3999.  The sequence has 19770 terms.
   a(1) = 7, a(2) = 3, … (see OEIS A222581).  The implementation
   builds the concatenation once and stores the run lengths in a
   static array for fast repeated queries. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *runs = NULL;      /* array of run lengths */
    static size_t run_count = 0;    /* number of runs stored */

    /* build the runs once */
    if (!runs) {
        /* allocate a buffer large enough for the known number of runs */
        size_t capacity = 20000;            /* > expected 19770 runs */
        runs = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!runs) return 0;

        run_count = 0;
        char cur_digit = '\0';
        seq_t cur_len = 0;

        for (int num = 1; num <= 3999; ++num) {
            int m = num;
            int thousands = m / 1000;  m %= 1000;
            int hundreds  = m / 100;   m %= 100;
            int tens      = m / 10;    int ones = m % 10;

            /* maximum length of a Roman numeral for 1..3999 is 15 */
            char buf[20];
            int pos = 0;

            /* thousands */
            for (int i = 0; i < thousands; ++i) buf[pos++] = 'M';

            /* hundreds */
            if (hundreds == 9)       { buf[pos++] = 'C'; buf[pos++] = 'M'; }
            else if (hundreds == 4)  { buf[pos++] = 'C'; buf[pos++] = 'D'; }
            else {
                if (hundreds >= 5) { buf[pos++] = 'D';
                    for (int i = 0; i < hundreds - 5; ++i) buf[pos++] = 'C'; }
                else {
                    for (int i = 0; i < hundreds; ++i) buf[pos++] = 'C';
                }
            }

            /* tens */
            if (tens == 9)           { buf[pos++] = 'X'; buf[pos++] = 'C'; }
            else if (tens == 4)      { buf[pos++] = 'X'; buf[pos++] = 'L'; }
            else {
                if (tens >= 5) { buf[pos++] = 'L';
                    for (int i = 0; i < tens - 5; ++i) buf[pos++] = 'X'; }
                else {
                    for (int i = 0; i < tens; ++i) buf[pos++] = 'X';
                }
            }

            /* ones */
            if (ones == 9)           { buf[pos++] = 'I'; buf[pos++] = 'X'; }
            else if (ones == 4)      { buf[pos++] = 'I'; buf[pos++] = 'V'; }
            else {
                if (ones >= 5) { buf[pos++] = 'V';
                    for (int i = 0; i < ones - 5; ++i) buf[pos++] = 'I'; }
                else {
                    for (int i = 0; i < ones; ++i) buf[pos++] = 'I';
                }
            }

            /* process the generated characters */
            for (int i = 0; i < pos; ++i) {
                char ch = buf[i];
                if (cur_digit == '\0') {
                    cur_digit = ch;
                    cur_len = 1;
                } else if (ch == cur_digit) {
                    ++cur_len;
                } else {
                    if (run_count >= capacity) {
                        capacity *= 2;
                        seq_t *tmp = (seq_t *)realloc(runs, capacity * sizeof(seq_t));
                        if (!tmp) { free(runs); return 0; }
                        runs = tmp;
                    }
                    runs[run_count++] = cur_len;
                    cur_digit = ch;
                    cur_len   = 1;
                }
            }
        }

        /* close the final run */
        if (cur_len > 0) {
            if (run_count >= capacity) {
                capacity++;
                seq_t *tmp = (seq_t *)realloc(runs, capacity * sizeof(seq_t));
                if (!tmp) { free(runs); return 0; }
                runs = tmp;
            }
            runs[run_count++] = cur_len;
        }
    }

    if (n < 1 || n > run_count) return 0;
    return runs[n - 1];
}
