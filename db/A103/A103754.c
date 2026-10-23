/*
 * A103754  Number of contiguous digits i in the counting numbers, for i=0.
 * https://oeis.org/A103754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the lengths of successive runs of zeros when writing the natural
 * numbers 0,1,2,3,… in decimal, producing the OEIS sequence A103754.
 * The algorithm proceeds by scanning each decimal representation in
 * order and recording the lengths of contiguous blocks of the digit 0.
 * The first block comes from the number 0 itself.
 */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n < 1 */

    /* Static state shared between successive calls. */
    static seq_t *runs = NULL;                /* array of run lengths */
    static size_t cap  = 0;                   /* current capacity      */
    static size_t count = 0;                  /* number of runs stored */
    static uint64_t next_num = 0;             /* next integer to process */

    /* Grow the array if needed. */
    if (cap == 0) {
        cap = 1024;
        runs = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!runs) return 0;                  /* allocation failure */
    }

    /* Process numbers until we have at least n runs. */
    while (count < n) {
        /* Obtain the decimal digits of next_num in reverse order. */
        uint64_t tmp = next_num;
        int digits[20];
        int len = 0;
        if (tmp == 0) {
            digits[len++] = 0;
        } else {
            while (tmp > 0 && len < 20) {
                digits[len++] = tmp % 10;
                tmp /= 10;
            }
        }

        /* Scan digits from most significant to least to detect runs. */
        int zero_run = 0;
        for (int i = len - 1; i >= 0; --i) {
            if (digits[i] == 0) {
                ++zero_run;
            } else {
                if (zero_run > 0) {
                    /* Store the run length. */
                    if (count >= cap) {
                        size_t new_cap = cap * 2;
                        seq_t *tmp_ptr = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
                        if (!tmp_ptr) return 0;          /* allocation failure */
                        runs = tmp_ptr;
                        cap = new_cap;
                    }
                    runs[count++] = (seq_t)zero_run;
                    zero_run = 0;
                }
            }
        }
        /* If the number ends with zeros, flush the run. */
        if (zero_run > 0) {
            if (count >= cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp_ptr = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
                if (!tmp_ptr) return 0;
                runs = tmp_ptr;
                cap = new_cap;
            }
            runs[count++] = (seq_t)zero_run;
        }

        ++next_num;                    /* advance to next integer */
    }

    return runs[n - 1];
}
